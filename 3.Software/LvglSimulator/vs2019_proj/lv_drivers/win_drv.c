/**
 * @file win_drv.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "win_drv.h"
#if USE_WINDOWS

#include <windows.h>
#include <windowsx.h>
#include "lvgl/lvgl.h"

#if LV_COLOR_DEPTH < 16
#error Windows driver only supports true RGB colors at this time
#endif

/**********************
 *       DEFINES
 **********************/

 #define WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void do_register(void);
static void win_drv_flush(lv_disp_t *drv, lv_area_t *area, const lv_color_t * color_p);
static void win_drv_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
static void win_drv_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static bool win_drv_read(lv_indev_t *drv, lv_indev_data_t * data);
static void msg_handler(void *param);

static COLORREF lv_color_to_colorref(const lv_color_t color);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


/**********************
 *  GLOBAL VARIABLES
 **********************/

bool lv_win_exit_flag = false;
lv_disp_t *lv_windows_disp;

/**********************
 *  STATIC VARIABLES
 **********************/
static HWND hwnd;
static uint32_t *fbp = NULL; /* Raw framebuffer memory */
static bool mouse_pressed;
static int mouse_x, mouse_y;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
const char g_szClassName[] = "LittlevGL";

HWND windrv_init(void)
{
    WNDCLASSEX wc;
    RECT winrect;
    HICON lvgl_icon;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    lvgl_icon        = (HICON) LoadImage( NULL, "lvgl_icon.bmp", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    if(lvgl_icon == NULL)
        lvgl_icon = LoadIcon(NULL, IDI_APPLICATION);

    wc.hIcon         = lvgl_icon;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = lvgl_icon;

    if(!RegisterClassEx(&wc))
    {
        return NULL;
    }

    winrect.left = 0;
    winrect.right = WINDOW_HOR_RES - 1;
    winrect.top = 0;
    winrect.bottom = WINDOW_VER_RES - 1;
    AdjustWindowRectEx(&winrect, WINDOW_STYLE, FALSE, WS_EX_CLIENTEDGE);
    OffsetRect(&winrect, -winrect.left, -winrect.top);
    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "LittlevGL Simulator",
        WINDOW_STYLE,
        CW_USEDEFAULT, CW_USEDEFAULT, winrect.right, winrect.bottom,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if(hwnd == NULL)
    {
        return NULL;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);


    lv_task_create(msg_handler, 0, LV_TASK_PRIO_HIGHEST, NULL);
    lv_win_exit_flag = false;
    do_register();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void do_register(void)
{
        /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /* LittlevGL requires a buffer where it draw the objects. The buffer's has to be greater than 1 display row
     *
     * There are three buffering configurations:
     * 1. Create ONE buffer some rows: LittlevGL will draw the display's content here and writes it to your display
     * 2. Create TWO buffer some rows: LittlevGL will draw the display's content to a buffer and writes it your display.
     *                                 You should use DMA to write the buffer's content to the display.
     *                                 It will enable LittlevGL to draw the next part of the screen to the other buffer while
     *                                 the data is being sent form the first buffer. It makes rendering and flushing parallel.
     * 3. Create TWO screen buffer: Similar to 2) but the buffer have to be screen sized. When LittlevGL is ready it will give the
     *                              whole frame to display. This way you only need to change the frame buffer's address instead of
     *                              copying the pixels.
     * */

    /* Example for 1) */
    static lv_disp_buf_t disp_buf_1;
    static lv_color_t buf1_1[WINDOW_HOR_RES * 10];                      /*A buffer for 10 rows*/
    lv_disp_buf_init(&disp_buf_1, buf1_1, NULL, WINDOW_HOR_RES * 10);   /*Initialize the display buffer*/


    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = WINDOW_HOR_RES;
    disp_drv.ver_res = WINDOW_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = win_drv_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf_1;

    /*Finally register the driver*/
    lv_windows_disp = lv_disp_drv_register(&disp_drv);
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = win_drv_read;
    lv_indev_drv_register(&indev_drv);
}

static void msg_handler(void *param)
{
    (void)param;

    MSG msg;
    BOOL bRet;
    if( (bRet = PeekMessage( &msg, NULL, 0, 0, TRUE )) != 0)
    {
        if (bRet == -1)
        {
            return;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if(msg.message == WM_QUIT)
            lv_win_exit_flag = true;
    }
}

 static bool win_drv_read(lv_indev_t *drv, lv_indev_data_t * data)
{
    data->state = mouse_pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    data->point.x = mouse_x;
    data->point.y = mouse_y;
    return false;
}

 static void on_paint(void)
 {
    HBITMAP bmp = CreateBitmap(WINDOW_HOR_RES, WINDOW_VER_RES, 1, 32, fbp);
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = SelectObject(hdcMem, bmp);

    BitBlt(hdc, 0, 0, WINDOW_HOR_RES, WINDOW_VER_RES, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);

    EndPaint(hwnd, &ps);
    DeleteObject(bmp);

}
/**
 * Flush a buffer to the marked area
 * @param x1 left coordinate
 * @param y1 top coordinate
 * @param x2 right coordinate
 * @param y2 bottom coordinate
 * @param color_p an array of colors
 */
static void win_drv_flush(lv_disp_t *drv, lv_area_t *area, const lv_color_t * color_p)
{
    win_drv_map(area->x1, area->y1, area->x2, area->y2, color_p);
    lv_disp_flush_ready(drv);
}

/**
 * Put a color map to the marked area
 * @param x1 left coordinate
 * @param y1 top coordinate
 * @param x2 right coordinate
 * @param y2 bottom coordinate
 * @param color_p an array of colors
 */
static void win_drv_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    for(int y = y1; y <= y2; y++)
    {
        for(int x = x1; x <= x2; x++)
        {
            fbp[y*WINDOW_HOR_RES+x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
    InvalidateRect(hwnd, NULL, FALSE);
    UpdateWindow(hwnd);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch(msg) {
    case WM_CREATE:
        fbp = malloc(4*WINDOW_HOR_RES*WINDOW_VER_RES);
        if(fbp == NULL)
            return 1;
        SetTimer(hwnd, 0, 10, (TIMERPROC)lv_task_handler);
        SetTimer(hwnd, 1, 25, NULL);

        return 0;
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        mouse_x = GET_X_LPARAM(lParam);
        mouse_y = GET_Y_LPARAM(lParam);
        if(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) {
            mouse_pressed = (msg == WM_LBUTTONDOWN);
        }
        return 0;
    case WM_CLOSE:
        free(fbp);
        fbp = NULL;
        DestroyWindow(hwnd);
        return 0;
    case WM_PAINT:
        on_paint();
        return 0;
    case WM_TIMER:
        lv_tick_inc(25);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
static COLORREF lv_color_to_colorref(const lv_color_t color)
{
    uint32_t raw_color = lv_color_to32(color);
    lv_color32_t tmp;
    tmp.full = raw_color;
    uint32_t colorref = RGB(tmp.ch.red, tmp.ch.green, tmp.ch.blue);
    return colorref;
}
#endif



