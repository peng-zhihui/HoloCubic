
# HoloCubic--多功能透明显示屏桌面站

**视频介绍：** https://www.bilibili.com/video/BV1VA411p7MD/

## 1. 固件编译说明

玩过Arduino的基本没有上手难度了，把Firmware/Libraries里面的库安装到Arduino库目录（如果你用的是Arduino IDE的话），然后这里需要修改一个官方库文件才能正常使用：

在ino文件中可以直接修其中的数据来显示自己地方的天气和粉丝数 

    String  appid = "XXXX";		        //天气API的APPID	 
    String  appsecret = "XXXX";		 //天气API的APPSecret
    String  cityid = "XXXX";		 //天气API的城市ID  
    String  biliid = "XXXX";		 //Bilibili的ID
    String  cityname = "XXXX";		 //城市名

 天气API的官网地址：https://www.tianqiapi.com   （可参考官网教程、城市ID等）

其他功能待更新！

首先肯定得安装ESP32的Arduino支持包（百度有海量教程），然后在安装的支持包的`esp32\hardware\esp32\1.0.4\libraries\SPI\src\SPI.cpp`文件中，**修改以下代码中的MISO为26**：

    if(sck == -1 && miso == -1 && mosi == -1 && ss == -1) {
        _sck = (_spi_num == VSPI) ? SCK : 14;
        _miso = (_spi_num == VSPI) ? MISO : 12; // 需要改为26
        _mosi = (_spi_num == VSPI) ? MOSI : 13;
        _ss = (_spi_num == VSPI) ? SS : 15;
这是因为，硬件上连接屏幕和SD卡分别是用两个硬件SPI，其中HSPI的默认MISO引脚是12，而12在ESP32中是用于上电时设置flash电平的，上电之前上拉会导致芯片无法启动，因此我们将默认的引脚替换为26。

> 也可以通过设置芯片熔丝的方式解决这个问题，不过那样的操作时一次性的，不建议这么玩。

**另外：**

Visual Studio的工程，可以在电脑上模拟LVGL的界面效果，改好之后代码粘贴到Arduino固件那边就可以完成界面移植。这样省的每次修改都要重新交叉编译Arduino的固件，提升开发效率。

## 其他的后续再补充，有用的话记得点星星~

