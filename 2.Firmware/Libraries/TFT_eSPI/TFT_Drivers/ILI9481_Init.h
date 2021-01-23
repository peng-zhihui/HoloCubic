
// This is the command sequence that initialises the ILI9481 driver
//
// This setup information uses simple 8 bit SPI writecommand() and writedata() functions
//
// See ST7735_Setup.h file for an alternative format

#define ILI9481_INIT_1 // Original default
//#define ILI9481_INIT_2 // CPT29
//#define ILI9481_INIT_3 // PVI35
//#define ILI9481_INIT_4 // AUO317
//#define ILI9481_INIT_5 // CMO35 *****
//#define ILI9481_INIT_6 // RGB

/////////////////////////////////////////////////////////////////////////////////////////
#ifdef ILI9481_INIT_1
// Configure ILI9481 display

    writecommand(TFT_SLPOUT);
    delay(20);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x42);
    writedata(0x18);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x07);
    writedata(0x10);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x02);

    writecommand(0xC0);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);

    writecommand(0xC5);
    writedata(0x03);

    writecommand(0xC8);
    writedata(0x00);
    writedata(0x32);
    writedata(0x36);
    writedata(0x45);
    writedata(0x06);
    writedata(0x16);
    writedata(0x37);
    writedata(0x75);
    writedata(0x77);
    writedata(0x54);
    writedata(0x0C);
    writedata(0x00);

    writecommand(TFT_MADCTL);
    writedata(0x0A);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif
    
    #ifndef TFT_PARALLEL_8_BIT
      writecommand(TFT_INVON);
    #endif

    writecommand(TFT_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(TFT_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);

    delay(120);
    writecommand(TFT_DISPON);

    delay(25);
// End of ILI9481 display configuration
/////////////////////////////////////////////////////////////////////////////////////////
#elif defined (ILI9481_INIT_2)
// Configure ILI9481 display

    writecommand(TFT_SLPOUT);
    delay(20);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x41);
    writedata(0x1D);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x2B);
    writedata(0x1F);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x11);

    writecommand(0xC0);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);

    writecommand(0xC5);
    writedata(0x03);

    writecommand(0xC8);
    writedata(0x00);
    writedata(0x14);
    writedata(0x33);
    writedata(0x10);
    writedata(0x00);
    writedata(0x16);
    writedata(0x44);
    writedata(0x36);
    writedata(0x77);
    writedata(0x00);
    writedata(0x0F);
    writedata(0x00);

    writecommand(0xB0);
    writedata(0x00);

    writecommand(0xE4);
    writedata(0xA0);
    
    writecommand(0xF0);
    writedata(0x01);

    writecommand(0xF3);
    writedata(0x02);
    writedata(0x1A);

    writecommand(TFT_MADCTL);
    writedata(0x0A);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif
    
    #ifndef TFT_PARALLEL_8_BIT
      writecommand(TFT_INVON);
    #endif

    writecommand(TFT_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(TFT_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);

    delay(120);
    writecommand(TFT_DISPON);

    delay(25);
// End of ILI9481 display configuration
/////////////////////////////////////////////////////////////////////////////////////////
#elif defined (ILI9481_INIT_3)
// Configure ILI9481 display

    writecommand(TFT_SLPOUT);
    delay(20);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x41);
    writedata(0x1D);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x2B);
    writedata(0x1F);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x11);

    writecommand(0xC0);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);

    writecommand(0xC5);
    writedata(0x03);

    writecommand(0xC8);
    writedata(0x00);
    writedata(0x14);
    writedata(0x33);
    writedata(0x10);
    writedata(0x00);
    writedata(0x16);
    writedata(0x44);
    writedata(0x36);
    writedata(0x77);
    writedata(0x00);
    writedata(0x0F);
    writedata(0x00);

    writecommand(0xB0);
    writedata(0x00);

    writecommand(0xE4);
    writedata(0xA0);
    
    writecommand(0xF0);
    writedata(0x01);

    writecommand(0xF3);
    writedata(0x40);
    writedata(0x0A);

    writecommand(TFT_MADCTL);
    writedata(0x0A);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif
    
    #ifndef TFT_PARALLEL_8_BIT
      writecommand(TFT_INVON);
    #endif

    writecommand(TFT_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(TFT_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);

    delay(120);
    writecommand(TFT_DISPON);

    delay(25);
// End of ILI9481 display configuration
/////////////////////////////////////////////////////////////////////////////////////////
#elif defined (ILI9481_INIT_4)
// Configure ILI9481 display

    writecommand(TFT_SLPOUT);
    delay(20);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x40);
    writedata(0x1D);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x18);
    writedata(0x13);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x11);

    writecommand(0xC0);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);

    writecommand(0xC5);
    writedata(0x03);

    writecommand(0xC8);
    writedata(0x00);
    writedata(0x44);
    writedata(0x06);
    writedata(0x44);
    writedata(0x0A);
    writedata(0x08);
    writedata(0x17);
    writedata(0x33);
    writedata(0x77);
    writedata(0x44);
    writedata(0x08);
    writedata(0x0C);

    writecommand(0xB0);
    writedata(0x00);

    writecommand(0xE4);
    writedata(0xA0);
    
    writecommand(0xF0);
    writedata(0x01);

    writecommand(TFT_MADCTL);
    writedata(0x0A);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif
    
    #ifndef TFT_PARALLEL_8_BIT
      writecommand(TFT_INVON);
    #endif

    writecommand(TFT_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(TFT_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);

    delay(120);
    writecommand(TFT_DISPON);

    delay(25);
// End of ILI9481 display configuration
/////////////////////////////////////////////////////////////////////////////////////////
#elif defined (ILI9481_INIT_5)
// Configure ILI9481 display

    writecommand(TFT_SLPOUT);
    delay(20);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x41);
    writedata(0x1D);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x1C);
    writedata(0x1F);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x11);

    writecommand(0xC0);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);

    writecommand(0xC5);
    writedata(0x03);

    writecommand(0xC6);
    writedata(0x83);

    writecommand(0xC8);
    writedata(0x00);
    writedata(0x26);
    writedata(0x21);
    writedata(0x00);
    writedata(0x00);
    writedata(0x1F);
    writedata(0x65);
    writedata(0x23);
    writedata(0x77);
    writedata(0x00);
    writedata(0x0F);
    writedata(0x00);

    writecommand(0xB0);
    writedata(0x00);

    writecommand(0xE4);
    writedata(0xA0);
    
    writecommand(0xF0);
    writedata(0x01);

    writecommand(TFT_MADCTL);
    writedata(0x0A);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif
    
    #ifndef TFT_PARALLEL_8_BIT
      writecommand(TFT_INVON);
    #endif

    writecommand(TFT_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(TFT_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);

    delay(120);
    writecommand(TFT_DISPON);

    delay(25);
// End of ILI9481 display configuration
/////////////////////////////////////////////////////////////////////////////////////////
#elif defined (ILI9481_INIT_6)
// Configure ILI9481 display

    writecommand(TFT_SLPOUT);
    delay(20);

    writecommand(0xD0);
    writedata(0x07);
    writedata(0x41);
    writedata(0x1D);

    writecommand(0xD1);
    writedata(0x00);
    writedata(0x2B);
    writedata(0x1F);

    writecommand(0xD2);
    writedata(0x01);
    writedata(0x11);

    writecommand(0xC0);
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);
    writedata(0x00);

    writecommand(0xC5);
    writedata(0x03);

    writecommand(0xC6);
    writedata(0x80);

    writecommand(0xC8);
    writedata(0x00);
    writedata(0x14);
    writedata(0x33);
    writedata(0x10);
    writedata(0x00);
    writedata(0x16);
    writedata(0x44);
    writedata(0x36);
    writedata(0x77);
    writedata(0x00);
    writedata(0x0F);
    writedata(0x00);

    writecommand(0xB0);
    writedata(0x00);

    writecommand(0xE4);
    writedata(0xA0);
    
    writecommand(0xF0);
    writedata(0x08);

    writecommand(0xF3);
    writedata(0x40);
    writedata(0x0A);

    writecommand(0xF6);
    writedata(0x84);

    writecommand(0xF7);
    writedata(0x80);

    writecommand(0xB3);
    writedata(0x00);
    writedata(0x01);
    writedata(0x06);
    writedata(0x30);

    writecommand(0xB4);
    writedata(0x00);

    writecommand(0x0C);
    writedata(0x00);
    writedata(0x55);

    writecommand(TFT_MADCTL);
    writedata(0x0A);

    writecommand(0x3A);
    #if defined (TFT_PARALLEL_8_BIT) || defined (RPI_DISPLAY_TYPE)
      writedata(0x55);           // 16 bit colour interface
    #else
      writedata(0x66);           // 18 bit colour interface
    #endif
    
    #ifndef TFT_PARALLEL_8_BIT
      writecommand(TFT_INVON);
    #endif

    writecommand(TFT_CASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(TFT_PASET);
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);

    delay(120);
    writecommand(TFT_DISPON);

    delay(25);
// End of ILI9481 display configuration
/////////////////////////////////////////////////////////////////////////////////////////
#endif