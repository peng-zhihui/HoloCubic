#ifndef RGB_H
#define RGB_H

#include <FastLED.h>

#define RGB_LED_NUM 2
#define RGB_LED_PIN 27


class Pixel
{
private:
	CRGB color_buffers[RGB_LED_NUM];

public:
	void init();

	Pixel& setRGB(int id, int r, int g, int b);
	Pixel& setBrightness(float duty);
};

#endif