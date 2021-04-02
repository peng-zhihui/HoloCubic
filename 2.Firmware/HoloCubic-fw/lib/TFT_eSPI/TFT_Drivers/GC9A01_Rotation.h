
// This is the command sequence that rotates the GC9A01 driver coordinate frame

  rotation = m % 4;

  writecommand(TFT_MADCTL);
  switch (rotation) {
    case 0: // Portrait
      writedata(TFT_MAD_BGR);
      _width  = _init_width;
      _height = _init_height;
      break;
    case 1: // Landscape (Portrait + 90)
      writedata(TFT_MAD_MX | TFT_MAD_MV | TFT_MAD_BGR);
      _width  = _init_height;
      _height = _init_width;
      break;
    case 2: // Inverter portrait
      writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_BGR);
      _width  = _init_width;
      _height = _init_height;
      break;
    case 3: // Inverted landscape
      writedata(TFT_MAD_MV | TFT_MAD_MY | TFT_MAD_BGR);
      _width  = _init_height;
      _height = _init_width;
      break;
  }
