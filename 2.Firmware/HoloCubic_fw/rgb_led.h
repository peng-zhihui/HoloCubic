#ifndef RGB_H
#define RGB_H

#include <FastLED.h>

#define RGB_LED_NUM 1
#define RGB_LED_PIN 27
 

class Pixel
{
private:
	CRGB color_buffers[RGB_LED_NUM];

public:
	void init();

	Pixel& setRGB(int r, int g, int b);
	Pixel& setBrightness(float duty);
};

#endif