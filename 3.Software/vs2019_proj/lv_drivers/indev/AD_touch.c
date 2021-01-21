/**
 * @file AD_touch.c
 *
 */

#include "AD_touch.h"

#if USE_AD_TOUCH

#include LV_DRV_INDEV_INCLUDE
#include LV_DRV_DELAY_INCLUDE

#define SAMPLE_POINTS   4

#define CALIBRATIONINSET 20 // range 0 <= CALIBRATIONINSET <= 40

#define RESISTIVETOUCH_AUTO_SAMPLE_MODE
#define TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD 350 // between 0-0x03ff the lesser this value


// Current ADC values for X and Y channels
int16_t adcX = 0;
int16_t adcY = 0;
volatile unsigned int adcTC = 0;

// coefficient values
volatile long _trA;
volatile long _trB;
volatile long _trC;
volatile long _trD;

volatile int16_t xRawTouch[SAMPLE_POINTS] = {TOUCHCAL_ULX, TOUCHCAL_URX, TOUCHCAL_LRX, TOUCHCAL_LLX};
volatile int16_t yRawTouch[SAMPLE_POINTS] = {TOUCHCAL_ULY, TOUCHCAL_URY, TOUCHCAL_LRY, TOUCHCAL_LLY};

#define TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR 8

// use this scale factor to avoid working in floating point numbers
#define SCALE_FACTOR (1 << TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)

typedef enum {
    IDLE, //0
    SET_X, //1
    RUN_X, //2
    GET_X, //3
    RUN_CHECK_X, //4
    CHECK_X, //5
    SET_Y, //6
    RUN_Y, //7
    GET_Y, //8
    CHECK_Y, //9
    SET_VALUES, //10
    GET_POT, //11
    RUN_POT //12
} TOUCH_STATES;

volatile TOUCH_STATES state = IDLE;

#define CAL_X_INSET (((GetMaxX() + 1) * (CALIBRATIONINSET >> 1)) / 100)
#define CAL_Y_INSET (((GetMaxY() + 1) * (CALIBRATIONINSET >> 1)) / 100)

int stat;
int16_t temp_x, temp_y;


static int16_t TouchGetX(void);
static int16_t TouchGetRawX(void);
static int16_t TouchGetY(void);
static int16_t TouchGetRawY(void);
static int16_t TouchDetectPosition(void);
static void TouchCalculateCalPoints(void);


/********************************************************************/
void ad_touch_init(void)
{
    // Initialize ADC for auto sampling mode
    AD1CON1 = 0;      // reset
    AD1CON2 = 0;      // AVdd, AVss, int every conversion, MUXA only
    AD1CON3 = 0x1FFF; // 31 Tad auto-sample, Tad = 256*Tcy
    AD1CON1 = 0x80E0; // Turn on A/D module, use auto-convert


    ADPCFG_XPOS = RESISTIVETOUCH_ANALOG;
    ADPCFG_YPOS = RESISTIVETOUCH_ANALOG;

    AD1CSSL = 0; // No scanned inputs

    state = SET_X; // set the state of the state machine to start the sampling

    /*Load calibration data*/
    xRawTouch[0] = TOUCHCAL_ULX;
    yRawTouch[0] = TOUCHCAL_ULY;
    xRawTouch[1] = TOUCHCAL_URX;
    yRawTouch[1] = TOUCHCAL_URY;
    xRawTouch[3] = TOUCHCAL_LLX;
    yRawTouch[3] = TOUCHCAL_LLY;
    xRawTouch[2] = TOUCHCAL_LRX;
    yRawTouch[2] = TOUCHCAL_LRY;

    TouchCalculateCalPoints();
}

/*Use this in lv_indev_drv*/
bool ad_touch_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static int16_t last_x = 0;
    static int16_t last_y = 0;

    int16_t x, y;

    x = TouchGetX();
    y = TouchGetY();

    if((x > 0) && (y > 0)) {
        data->point.x = x;
        data->point.y = y;
        last_x = data->point.x;
        last_y = data->point.y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
    }

    return false;
}

/* Call periodically (e.g. in every 1 ms) to handle reading with ADC*/
int16_t ad_touch_handler(void)
{
    static int16_t tempX, tempY;
    int16_t temp;

    switch(state) {
        case IDLE:
            adcX = 0;
            adcY = 0;
            break;

        case SET_VALUES:
            if(!TOUCH_ADC_DONE)
                break;
            if((WORD)TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD < (WORD)ADC1BUF0) {
                adcX = 0;
                adcY = 0;
            } else {
                adcX = tempX;
                adcY = tempY;
            }
            state = SET_X;
            return 1; // touch screen acquisition is done

        case SET_X:
            TOUCH_ADC_INPUT_SEL = ADC_XPOS;

            ResistiveTouchScreen_XPlus_Config_As_Input();
            ResistiveTouchScreen_YPlus_Config_As_Input();
            ResistiveTouchScreen_XMinus_Config_As_Input();
            ResistiveTouchScreen_YMinus_Drive_Low();
            ResistiveTouchScreen_YMinus_Config_As_Output();

            ADPCFG_YPOS = RESISTIVETOUCH_DIGITAL; // set to digital pin
            ADPCFG_XPOS = RESISTIVETOUCH_ANALOG; // set to analog pin

            TOUCH_ADC_START = 1; // run conversion
            state = CHECK_X;
            break;

        case CHECK_X:
        case CHECK_Y:

            if(TOUCH_ADC_DONE == 0) {
                break;
            }

            if((WORD)TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD > (WORD)ADC1BUF0) {
                if(state == CHECK_X) {
                    ResistiveTouchScreen_YPlus_Drive_High();
                    ResistiveTouchScreen_YPlus_Config_As_Output();
                    tempX = 0;
                    state = RUN_X;
                } else {
                    ResistiveTouchScreen_XPlus_Drive_High();
                    ResistiveTouchScreen_XPlus_Config_As_Output();
                    tempY = 0;
                    state = RUN_Y;
                }
            } else {
                adcX = 0;
                adcY = 0;
                state = SET_X;
                return 1; // touch screen acquisition is done
                break;
            }

        case RUN_X:
        case RUN_Y:
            TOUCH_ADC_START = 1;
            state = (state == RUN_X) ? GET_X : GET_Y;
            // no break needed here since the next state is either GET_X or GET_Y
            break;

        case GET_X:
        case GET_Y:
            if(!TOUCH_ADC_DONE)
                break;

            temp = ADC1BUF0;
            if(state == GET_X) {
                if(temp != tempX) {
                    tempX =  temp;
                    state = RUN_X;
                    break;
                }
            } else {
                if(temp != tempY) {
                    tempY = temp;
                    state = RUN_Y;
                    break;
                }
            }

            if(state == GET_X)
                ResistiveTouchScreen_YPlus_Config_As_Input();
            else
                ResistiveTouchScreen_XPlus_Config_As_Input();
            TOUCH_ADC_START = 1;
            state = (state == GET_X) ? SET_Y : SET_VALUES;
            break;

        case SET_Y:
            if(!TOUCH_ADC_DONE)
                break;

            if((WORD)TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD < (WORD)ADC1BUF0) {
                adcX = 0;
                adcY = 0;
                state = SET_X;
                return 1; // touch screen acquisition is done
                break;
            }

            TOUCH_ADC_INPUT_SEL = ADC_YPOS;

            ResistiveTouchScreen_XPlus_Config_As_Input();
            ResistiveTouchScreen_YPlus_Config_As_Input();
            ResistiveTouchScreen_XMinus_Drive_Low();
            ResistiveTouchScreen_XMinus_Config_As_Output();
            ResistiveTouchScreen_YMinus_Config_As_Input();

            ADPCFG_YPOS = RESISTIVETOUCH_ANALOG; // set to analog pin
            ADPCFG_XPOS = RESISTIVETOUCH_DIGITAL; // set to digital pin
            TOUCH_ADC_START = 1; // run conversion
            state = CHECK_Y;
            break;

        default:
            state = SET_X;
            return 1; // touch screen acquisition is done
    }
    stat = state;
    temp_x = adcX;
    temp_y = adcY;

    return 0; // touch screen acquisition is not done
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/********************************************************************/
static int16_t TouchGetX(void)
{
    long result;

    result = TouchGetRawX();

    if(result > 0) {
        result = (long)((((long)_trC * result) + _trD) >> TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR);

    }
    return ((int16_t)result);
}
/********************************************************************/
static int16_t TouchGetRawX(void)
{
#ifdef TOUCHSCREEN_RESISTIVE_SWAP_XY
    return adcY;
#else
    return adcX;
#endif
}

/********************************************************************/
static int16_t TouchGetY(void)
{

    long result;

    result = TouchGetRawY();

    if(result > 0) {
        result = (long)((((long)_trA * result) + (long)_trB) >> TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR);

    }
    return ((int16_t)result);
}

/********************************************************************/
static int16_t TouchGetRawY(void)
{
#ifdef TOUCHSCREEN_RESISTIVE_SWAP_XY
    return adcX;
#else
    return adcY;
#endif
}


static void TouchCalculateCalPoints(void)
{
    long trA, trB, trC, trD; // variables for the coefficients
    long trAhold, trBhold, trChold, trDhold;
    long test1, test2; // temp variables (must be signed type)

    int16_t xPoint[SAMPLE_POINTS], yPoint[SAMPLE_POINTS];

    yPoint[0] = yPoint[1] = CAL_Y_INSET;
    yPoint[2] = yPoint[3] = (GetMaxY() - CAL_Y_INSET);
    xPoint[0] = xPoint[3] = CAL_X_INSET;
    xPoint[1] = xPoint[2] = (GetMaxX() - CAL_X_INSET);

    // calculate points transfer functiona
    // based on two simultaneous equations solve for the
    // constants

    // use sample points 1 and 4
    // Dy1 = aTy1 + b; Dy4 = aTy4 + b
    // Dx1 = cTx1 + d; Dy4 = aTy4 + b

    test1 = (long)yPoint[0] - (long)yPoint[3];
    test2 = (long)yRawTouch[0] - (long)yRawTouch[3];

    trA = ((long)((long)test1 * SCALE_FACTOR) / test2);
    trB = ((long)((long)yPoint[0] * SCALE_FACTOR) - (trA * (long)yRawTouch[0]));

    test1 = (long)xPoint[0] - (long)xPoint[2];
    test2 = (long)xRawTouch[0] - (long)xRawTouch[2];

    trC = ((long)((long)test1 * SCALE_FACTOR) / test2);
    trD = ((long)((long)xPoint[0] * SCALE_FACTOR) - (trC * (long)xRawTouch[0]));

    trAhold = trA;
    trBhold = trB;
    trChold = trC;
    trDhold = trD;

    // use sample points 2 and 3
    // Dy2 = aTy2 + b; Dy3 = aTy3 + b
    // Dx2 = cTx2 + d; Dy3 = aTy3 + b

    test1 = (long)yPoint[1] - (long)yPoint[2];
    test2 = (long)yRawTouch[1] - (long)yRawTouch[2];

    trA = ((long)(test1 * SCALE_FACTOR) / test2);
    trB = ((long)((long)yPoint[1] * SCALE_FACTOR) - (trA * (long)yRawTouch[1]));

    test1 = (long)xPoint[1] - (long)xPoint[3];
    test2 = (long)xRawTouch[1] - (long)xRawTouch[3];

    trC = ((long)((long)test1 * SCALE_FACTOR) / test2);
    trD = ((long)((long)xPoint[1] * SCALE_FACTOR) - (trC * (long)xRawTouch[1]));

    // get the average and use the average
    _trA = (trA + trAhold) >> 1;
    _trB = (trB + trBhold) >> 1;
    _trC = (trC + trChold) >> 1;
    _trD = (trD + trDhold) >> 1;
}

#endif
