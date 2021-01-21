/**
 * @file AD_touch.h
 *
 */

#ifndef AD_TOUCH_H
#define AD_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_AD_TOUCH

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#define  _SUPPRESS_PLIB_WARNING
#include <plib.h>

#include "GenericTypeDefs.h"

#define DISP_ORIENTATION    0
#define DISP_HOR_RESOLUTION 320
#define DISP_VER_RESOLUTION 240

/*GetMaxX Macro*/
#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
#define GetMaxX()   (DISP_VER_RESOLUTION - 1)
#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
#define GetMaxX()   (DISP_HOR_RESOLUTION - 1)
#endif

/*GetMaxY Macro*/
#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)
#define GetMaxY()   (DISP_HOR_RESOLUTION - 1)
#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
#define GetMaxY()   (DISP_VER_RESOLUTION - 1)
#endif

/*********************************************************************
 * HARDWARE PROFILE FOR THE RESISTIVE TOUCHSCREEN
 *********************************************************************/

#define TOUCH_ADC_INPUT_SEL AD1CHS

// ADC Sample Start
#define TOUCH_ADC_START AD1CON1bits.SAMP

// ADC Status
#define TOUCH_ADC_DONE AD1CON1bits.DONE

#define RESISTIVETOUCH_ANALOG 1
#define RESISTIVETOUCH_DIGITAL 0

// ADC channel constants
#define ADC_XPOS ADC_CH0_POS_SAMPLEA_AN12
#define ADC_YPOS ADC_CH0_POS_SAMPLEA_AN13

// ADC Port Control Bits
#define ADPCFG_XPOS AD1PCFGbits.PCFG12 //XR
#define ADPCFG_YPOS AD1PCFGbits.PCFG13 //YD

// X port definitions
#define ResistiveTouchScreen_XPlus_Drive_High()     LATBbits.LATB12 = 1
#define ResistiveTouchScreen_XPlus_Drive_Low()      LATBbits.LATB12 = 0         //LAT_XPOS
#define ResistiveTouchScreen_XPlus_Config_As_Input()    TRISBbits.TRISB12 = 1 //TRIS_XPOS
#define ResistiveTouchScreen_XPlus_Config_As_Output()   TRISBbits.TRISB12 = 0

#define ResistiveTouchScreen_XMinus_Drive_High()    LATFbits.LATF0 = 1
#define ResistiveTouchScreen_XMinus_Drive_Low()     LATFbits.LATF0 = 0         //LAT_XNEG
#define ResistiveTouchScreen_XMinus_Config_As_Input()   TRISFbits.TRISF0 = 1 //TRIS_XNEG
#define ResistiveTouchScreen_XMinus_Config_As_Output()  TRISFbits.TRISF0 = 0

// Y port definitions
#define ResistiveTouchScreen_YPlus_Drive_High() LATBbits.LATB13 = 1
#define ResistiveTouchScreen_YPlus_Drive_Low()  LATBbits.LATB13 = 0         //LAT_YPOS
#define ResistiveTouchScreen_YPlus_Config_As_Input()    TRISBbits.TRISB13 = 1 //TRIS_YPOS
#define ResistiveTouchScreen_YPlus_Config_As_Output()   TRISBbits.TRISB13 = 0

#define ResistiveTouchScreen_YMinus_Drive_High() LATFbits.LATF1 = 1
#define ResistiveTouchScreen_YMinus_Drive_Low()     LATFbits.LATF1 = 0         //LAT_YNEG
#define ResistiveTouchScreen_YMinus_Config_As_Input()   TRISFbits.TRISF1 = 1 //TRIS_YNEG
#define ResistiveTouchScreen_YMinus_Config_As_Output()  TRISFbits.TRISF1 = 0

// Default calibration points
#define TOUCHCAL_ULX 0x0348
#define TOUCHCAL_ULY 0x00CC
#define TOUCHCAL_URX 0x00D2
#define TOUCHCAL_URY 0x00CE
#define TOUCHCAL_LLX 0x034D
#define TOUCHCAL_LLY 0x0335
#define TOUCHCAL_LRX 0x00D6
#define TOUCHCAL_LRY 0x032D

void ad_touch_init(void);
bool ad_touch_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
int16_t ad_touch_handler(void);

#endif /* USE_AD_TOUCH */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AD_TOUCH_H */
