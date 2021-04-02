
// This is the command sequence that initialises the ILI9486 driver
//
// This setup information uses simple 8 bit SPI writecommand() and writedata() functions
//
// See ST7735_Setup.h file for an alternative format

{
// From https://github.com/notro/fbtft/blob/master/fb_ili9486.c

    //writecommand(0x01); // SW reset
    //delay(120);
	
    writecommand(0x11); // Sleep out, also SW reset
    delay(120);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif

    writecommand(0xC2);
    writedata(0x44);

    writecommand(0xC5);
    writedata(0x00);
    writedata(0x00);
    writedata(0x00);
    writedata(0x00);

    writecommand(0xE0);
    writedata(0x0F);
    writedata(0x1F);
    writedata(0x1C);
    writedata(0x0C);
    writedata(0x0F);
    writedata(0x08);
    writedata(0x48);
    writedata(0x98);
    writedata(0x37);
    writedata(0x0A);
    writedata(0x13);
    writedata(0x04);
    writedata(0x11);
    writedata(0x0D);
    writedata(0x00);
 
    writecommand(0xE1);
    writedata(0x0F);
    writedata(0x32);
    writedata(0x2E);
    writedata(0x0B);
    writedata(0x0D);
    writedata(0x05);
    writedata(0x47);
    writedata(0x75);
    writedata(0x37);
    writedata(0x06);
    writedata(0x10);
    writedata(0x03);
    writedata(0x24);
    writedata(0x20);
    writedata(0x00);
 
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writecommand(TFT_INVOFF);
    #else
      writecommand(TFT_INVON);
    #endif
 
    writecommand(0x36);
    writedata(0x48);

    writecommand(0x29);                     // display on
    delay(150);
}
