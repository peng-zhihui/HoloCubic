// See SetupX_Template.h for all options available

#define EPD_DRIVER // ePaper driver


// READ THIS  READ THIS  READ THIS  READ THIS  READ THIS  READ THIS
// Install the ePaper library for your own display size and type
// from here:
// https://github.com/Bodmer/EPD_Libraries

//  Note: Pin allocations for the ePaper signals are defined in
//  the ePaper library's epdif.h file. There follows the default
//  pins already included in epdif.h file for the ESP8266:

///////////////////////////////////////////////////////////////////
//   For ESP8266 connect as follows:                             //
//   Display  3.3V to NodeMCU 3V3                                //
//   Display   GND to NodeMCU GND                                //
//                                                               //
//   Display   GPIO   NodeMCU pin                                //
//    BUSY       5      D1                                       //
//    RESET      4      D2                                       //
//    DC         0      D3                                       //
//    CS         2      D4                                       //
//    CLK       14      D5                                       //
//                      D6 (MISO not connected to display)       //
//    DIN       13      D7                                       //
//                                                               //
///////////////////////////////////////////////////////////////////


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
