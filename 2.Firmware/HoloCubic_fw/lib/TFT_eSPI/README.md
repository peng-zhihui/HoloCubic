A new ["Discussions"](https://github.com/Bodmer/TFT_eSPI/discussions) facility has been added for Q&A etc. Use the ["Issues"](https://github.com/Bodmer/TFT_eSPI/issues) tab only for problems with the library. Thanks!
# News
1. Viewports can now be applied to sprites e.g. spr.setViewport(5, 5, 20, 20); so graphics can be restricted to a particular area of the sprite. This operates in the same way as the TFT viewports, see 2. below.

2. The library now provides a "viewport" capability. See "Viewport_Demo" and "Viewport_graphicstest" examples. When a viewport is defined graphics will only appear within that window.  The coordinate datum by default moves to the top left corner of the viewport, but can optionally remain at top left corner of TFT. The GUIslice library will make use of this feature to speed up the rendering of GUI objects ([see #769](https://github.com/Bodmer/TFT_eSPI/issues/769)).

3. The library now supports SSD1963 based screen, this has been tested on a [480x800 screen](https://www.buydisplay.com/7-tft-screen-touch-lcd-display-module-w-ssd1963-controller-board-mcu) with an ESP32. The interface is 8 bit parallel only as that controller does not support a SPI interface.

4. A companion library [U8g2_for_TFT_eSPI](https://github.com/Bodmer/U8g2_for_TFT_eSPI) has been created to allow U8g2 library fonts to be used with TFT_eSPI.

5. The library now supports SPI DMA transfers for both ESP32 and STM32 processors. The DMA Test examples now work on the ESP32 for SPI displays (excluding RPi type and ILI9488).

6. A new option has been added for STM32 processors to optimise performance where Port A (or B) pins 0-7 are used for the 8 bit parallel interface data pins 0-7 to the TFT. This gives a dramatic 8 times better rendering performance for the lower clock rate STM32 processors such as the STM32F103 "Blue Pill" or STM411 "Black Pill" since no time consuming data bit manipulation is required. See setup file "User_Setups/Setup35_ILI9341_STM32_Port_Bus.h".

7. A new "Animated_dial" example has been added to show how dials can be created using a rotated Sprite for the needle. To run this example the TFT must support reading from the screen RAM. The dial rim and scale is a jpeg image, created using a paint program.

      ![Animated_dial](https://i.imgur.com/S736Rg6.png)

8. Anti-aliased (smooth) fonts can now be stored as arrays in FLASH (program) memory. This means that processors such as STM32 that do not have SPIFFS support can use the fonts. The processor must have sufficient FLASH memory to store the fonts used.


# TFT_eSPI

An Arduino IDE compatible graphics and fonts library for 32 bit processors. The library is targetted at 32 bit processors, it  has been performance optimised for STM32, ESP8266 and ESP32 types. The library can be loaded using the Arduino IDE's Library Manager. Direct Memory Access (DMA) can be used with the ESP32 and STM32 processors to improve rendering performance.

"Four wire" SPI and 8 bit parallel interfaces are supported. Due to lack of GPIO pins the 8 bit parallel interface is NOT supported on the ESP8266. 8 bit parallel interface TFTs  (e.g. UNO format mcufriend shields) can used with the STM32 Nucleo 64/144 range or the UNO format ESP32 (see below for ESP32).

Displays using the following controllers are supported:

* ILI9163
* ILI9225
* ILI9341
* ILI9481
* ILI9486
* ILI9488
* HX8357D
* S6D02A1
* SSD1963
* ST7735
* ST7789
* ST7796

ILI9341 and ST7796 SPI based displays are recommended as starting point for experimenting with this library.

The library supports some TFT displays designed for the Raspberry Pi (RPi) that are based on a ILI9486 or ST7796 driver chip with a 480 x 320 pixel screen. The ILI9486 RPi display must be of the Waveshare design and use a 16 bit serial interface based on the 74HC04, 74HC4040 and 2 x 74HC4094 logic chips. Note that due to design variations between these displays not all RPi displays will work with this library, so purchaing a RPi display of these types soley for use with this library is not recommended.

The "good" RPi displays are the [MHS-4.0 inch Display-B type ST7796](http://www.lcdwiki.com/MHS-4.0inch_Display-B) and [Waveshare 3.5 inch ILI9486 Type C](https://www.waveshare.com/wiki/3.5inch_RPi_LCD_(C)) displays are supported and provides good performance. These have a dedicated controller and can be clocked at up to 80MHz with the ESP32 (55MHz with STM32 and 40MHz with ESP8266).

Some displays permit the internal TFT screen RAM to be read, some of the examples use this feature. The TFT_Screen_Capture example allows full screens to be captured and sent to a PC, this is handy to create program documentation.

The library supports Waveshare 2 and 3 colour ePaper displays using full frame buffers. This addition is relatively immature and thus only one example has been provided.

The library includes a "Sprite" class, this enables flicker free updates of complex graphics. Direct writes to the TFT with graphics functions are still available, so existing sketches do not need to be changed.

# Sprites

A Sprite is notionally an invisible graphics screen that is kept in the processors RAM. Graphics can be drawn into the Sprite just as they can be drawn directly to the screen. Once the Sprite is completed it can be plotted onto the screen in any position. If there is sufficient RAM then the Sprite can be the same size as the screen and used as a frame buffer. Sprites by default use 16 bit colours, the bit depth can be set to 8 bits (256 colours) , or 1 bit (any 2 colours) to reduce the RAM needed. On an ESP8266 the largest 16 bit colour Sprite that can be created is about 160x128 pixels, this consumes 40Kbytes of RAM. On an ESP32 the workspace RAM is more limited than the datsheet implies so a 16 bit colour Sprite is limited to about 200x200 pixels (~80Kbytes), an 8 bit sprite to 320x240 pixels (~76kbytes). A 1 bit per pixel Sprite requires only 9600 bytes for a full 320 x 240 screen buffer, this is ideal for supporting use with 2 colour bitmap fonts.

One or more sprites can be created, a sprite can be any pixel width and height, limited only by available RAM. The RAM needed for a 16 bit colour depth Sprite is (2 x width x height) bytes, for a Sprite with 8 bit colour depth the RAM needed is (width x height) bytes. Sprites can be created and deleted dynamically as needed in the sketch, this means RAM can be freed up after the Sprite has been plotted on the screen, more RAM intensive WiFi based code can then be run and normal graphics operations still work.

Drawing graphics into a sprite is very fast, for those familiar with the Adafruit "graphicstest" example, this whole test completes in 18ms in a 160x128 sprite. Examples of sprite use can be found in the "examples/Sprite" folder.

Sprites can be plotted to the TFT with one colour being specified as "transparent", see Transparent_Sprite_Demo example.

If an ESP32 board has SPIRAM (i.e. PSRAM) fitted then Sprites will use the PSRAM memory and large full screen buffer Sprites can be created. Full screen Sprites take longer to render (~45ms for a 320 x 240 16 bit Sprite), so bear that in mind.

# Touch controller support

The XPT2046 touch screen controller is supported for SPI based displays only. The SPI bus for the touch controller is shared with the TFT and only an additional chip select line is needed. This support will eventually be deprecated when a suitable touch screen library is avaiable.

The Button class from Adafruit_GFX is incorporated, with the enhancement that the button labels can be in any font.

# ESP8266 overlay mode

The library supports SPI overlap on the ESP8266 so the TFT screen can share MOSI, MISO and SCLK pins with the program FLASH, this frees up GPIO pins for other uses.


# Fonts

The library contains proportional fonts, different sizes can be enabled/disabled at compile time to optimise the use of FLASH memory. Anti-alased (smooth) font files in vlw format stored in SPIFFS are supported. Any 16 bit Unicode character can be included and rendered, this means many language specific characters can be rendered to the screen.

The library is based on the Adafruit GFX and Adafruit driver libraries and the aim is to retain compatibility. Significant additions have been made to the library to boost the speed for ESP8266/ESP32 processors (it is typically 3 to 10 times faster) and to add new features. The new graphics functions include different size proportional fonts and formatting features. There are lots of example sketches to demonstrate the different features and included functions.

Configuration of the library font selections, pins used to interface with the TFT and other features is made by editting the User_Setup.h file in the library folder, or by selecting your own configuration in the "User_Setup_Selet,h" file.  Fonts and features can easily be enabled/disabled by commenting out lines.


# Anti-aliased Fonts

Anti-aliased (smooth) font files in "vlw" format are generated by the free [Processing IDE](https://processing.org/) using a sketch included in the library Tools folder. This sketch with the Processing IDE can be used to generate font files from your computer's font set or any TrueType (.ttf) font, the font file can include **any** combination of 16 bit Unicode characters. This means Greek, Japanese and any other UCS-2 glyphs can be used. Character arrays and Strings in UTF-8 format are supported.

Here is the Adafruit_GFX "FreeSans12pt" bitmap font compared to the same font drawn as anti-aliased:

![Smooth_font](https://i.imgur.com/gAeDPFY.png)

The smooth font example displays the following screen:

![Example](https://i.imgur.com/xJF0Oz7.png)

It would be possible to compress the vlw font files but the rendering performance to a TFT is still good when storing the font file(s) in SPIFFS.

Here is an example screenshot showing the anti-aliased Hiragana character Unicode block (0x3041 to 0x309F) in 24pt from the Microsoft Yahei font:

![Hiragana glyphs](https://i.imgur.com/jeXf2st.png)

Antialiased fonts can also be drawn over a gradient background with a callback to fetch the background colour of each pixel. This pixel colour can be set by the gradient algorithm or by reading back the TFT screen memory (if reading the display is supported).

# 8 bit parallel support

The common 8 bit "Mcufriend" shields are supported for the STM Nucleo 64/144 boards and ESP32 UNO style board. The STM32 "Blue/Black Pill" boards can also be used with 8 bit parallel displays.

The ESP32 board I have been using for testing has the following pinout:

![Example](https://i.imgur.com/bvM6leE.jpg)

UNO style boards with a Wemos R32(ESP32) label are also available at low cost with the same pin-out.

Unfortunately the typical UNO/mcufriend TFT display board maps LCD_RD, LCD_CS and LCD_RST signals to the ESP32 analogue pins 35, 34 and 36 which are input only.  To solve this I linked in the 3 spare pins IO15, IO33 and IO32 by adding wires to the bottom of the board as follows:

IO15 wired to IO35

IO33 wired to IO34

IO32 wired to IO36

![Example](https://i.imgur.com/pUZn6lF.jpg)

If the display board is fitted with a resistance based touch screen then this can be used by performing the modifications described here and the fork of the Adafruit library:
https://github.com/s60sc/Adafruit_TouchScreen

# Tips
If you load a new copy of TFT_eSPI then it will over-write your setups if they are kept within the TFT_eSPI folder. One way around this is to create a new folder in your Arduino library folder called "TFT_eSPI_Setups". You then place your custom setup.h files in there. After an upgrade simply edit the User_Setup_Select.h file to point to your custom setup file e.g.:
```
#include <../TFT_eSPI_Setups/my_custom_setup.h>
```
You must make sure only one setup file is called. In the the custom setup file I add the file path as a commented out first line that can be cut and pasted back into the upgraded User_Setup_Select.h file.  The ../ at the start of the path means go up one directory level. Clearly you could use different file paths or directory names as long as it does not clash with another library or folder name.

You can take this one step further and have your own setup select file and then you only need to replace the Setup.h line reference in User_Setup_Select.h to, for example:
```
#include <../TFT_eSPI_Setups/my_setup_select.h>
```
To select a new setup you then edit your own my_setup_select.h file (which will not get over-written during an upgrade).

# ePaper displays

The library was intended to support only TFT displays but using a Sprite as a 1 bit per pixel screen buffer permits support for the Waveshare 2 and 3 colour SPI ePaper displays. This addition to the library is experimental and only one example is provided. Further examples will be added.

![Example](https://i.imgur.com/L2tV129.jpg?1)

