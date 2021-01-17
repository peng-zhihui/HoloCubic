# HoloCubic
D:\Green\Arduino\arduino-1.8.7\portable\packages\esp32\hardware\esp32\1.0.4\libraries\SPI\src\SPI.cpp



    if(sck == -1 && miso == -1 && mosi == -1 && ss == -1) {
        _sck = (_spi_num == VSPI) ? SCK : 14;
        _miso = (_spi_num == VSPI) ? MISO : 12;
        _mosi = (_spi_num == VSPI) ? MOSI : 13;
        _ss = (_spi_num == VSPI) ? SS : 15;
MISO改成26



