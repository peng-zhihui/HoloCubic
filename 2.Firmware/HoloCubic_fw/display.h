#include <lvgl.h>

#define LCD_BL_PIN 5
#define LCD_BL_PWM_CHANNEL 0


class Display
{
private:


public:
	void init();
	void routine();
	void setBackLight(float);
};