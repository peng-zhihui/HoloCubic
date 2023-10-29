![](/5.Docs/Images/Holo1.jpg)

# HoloCubic--a multifunctional transparent display desktop station

**Video introduction:**<https://www.bilibili.com/video/BV1VA411p7MD/>

## 0. About this project

> Note from Zhihui Jun: This is an interesting gadget that I came up with over a weekend to update the video and relieve myself from the embarrassment of delaying the update for two months: D

As mentioned in the video, the interesting part of this project is the use of a beam splitter prism to design`伪全息显示`Effect. This small device has many functions in general. Because it is equipped with WiFi and Bluetooth capabilities, it can realize many network applications. In this warehouse, we provide you with a development framework and some basic functions (weather, fan count monitor, etc.). I can continue to expand and implement more applications based on my solution.

The hardware solution of this project is based on`ESP32PICO-D4`Yes, a very practical MCU chip from Espressif Systems. Due to the SiP package, the entire PCBA board area can be the size of a coin; the software is mainly based on`lvgl-GUI`Library, I transplanted ST7789 1.3 inch`240x240`resolution screen display driver, while also`MPU6050`As an input device, the encoder key value is simulated through induction to interact.

## 1. Hardware Proofing Instructions

**When it comes to PCB prototyping, I haven’t found anything that needs special attention yet.**The PCB file can be taken directly to the factory for proofing. Two-layer boards are very cheap, and the device BOM is also relatively common. The cost of the entire board is less than 50 yuan.

`Hardware`The file currently contains two versions of the PCB circuit:

-   **Naive Version**: The version that appears in the video, with onboard ESP32, IMU, ambient light sensor, SD card slot, download circuit, and two RGB lights
-   **Ironman Version**: Based on the above version, slightly modified, the ambient light sensor has been deleted, and the PCB shape has been modified to fit the new housing.

> Because the new shell is planned to use CNC for metal processing, the ambient light is easily blocked, and there are not many usage scenarios for this function, so it has been deleted in the new version.

**Shell processing**Choose according to the version you like,`3D Model`The folder currently contains four versions of the shell files:

-   **Naive Version**: The version that appears in the video is relatively simple (designed due to temporary rush) and is best processed using light-curing 3D printing.

    ![](/5.Docs/Images/Holo3.png)

-   **Bilibili Version**: The shell structure in the form of the top 100 trophies of Station B that appears later in the video is suitable for`Naive Version`PCB,**It is entertainment in nature and is not recommended for non-top 100 UPs.**。

-   **Metal Version**: After the release of the video, the newly revised shell structure design optimizes the layout and controls. The overall design is more compact, refined and adaptable.`Naive Version`PCB, it is recommended to use CNC processing

    ![](/5.Docs/Images/Holo2.jpg)

-   **Ironman Version**: Newly designed Wild Iron Man style structural parts. This version was designed in cooperation with a friend. He may be authorized to co-brand and mass-produce it later. This structure is suitable for`Ironman Version`PCB

    ![](/5.Docs/Images/Holo.jpg)

> The processing of the structural parts of the Wild Iron Man co-branded version is relatively complicated and requires post-processing such as sandblasting and anodizing, so the cost of manufacturing a single piece is very high (I found out that a complete set of 3 parts costs at least 1,000 yuan+), so everyone has to process it themselves. Channels can do it themselves using the provided documents.
>
> If you don’t have the channel but want this version of hardware,**I authorized that friend to mass produce a small batch**, his shop is called[Xiii](https://shop68240117.taobao.com), is a geek who is very experienced in making customized keyboards. If you are interested, you can pay attention~

## 2. Firmware compilation instructions

The firmware framework is mainly developed based on Arduino. If you have played with Arduino, it is basically not difficult to get started. Just install the libraries in Firmware/Libraries to the Arduino library directory (if you are using Arduino IDE).

> I use the Visual Micro plug-in in Visual Studio for Arduino development. Because I am familiar with VS, you can just choose your favorite IDE.

**Then you need to modify an official library file to use it normally:**

First of all, you must install the Arduino support package for ESP32 (Baidu has a lot of tutorials), and then in the installed support package`esp32\hardware\esp32\1.0.4\libraries\SPI\src\SPI.cpp`In the file,**Modify MISO in the following code to 26**：

    if(sck == -1 && miso == -1 && mosi == -1 && ss == -1) {
        _sck = (_spi_num == VSPI) ? SCK : 14;
        _miso = (_spi_num == VSPI) ? MISO : 12; // 需要改为26
        _mosi = (_spi_num == VSPI) ? MOSI : 13;
        _ss = (_spi_num == VSPI) ? SS : 15;

这是因为，硬件上连接屏幕和SD卡分别是用两个硬件SPI，其中HSPI的默认MISO引脚是12，而12在ESP32中是用于上电时设置flash电平的，上电之前上拉会导致芯片无法启动，因此我们将默认的引脚替换为26。

> This problem can also be solved by setting the chip fuse, but such an operation is one-time irreversible and is not recommended.

**in addition:**

Since I was busy with video production, the code was written temporarily and was very messy with a lot of dirty code. Therefore, what is in the warehouse is the template code after all the drivers have been adjusted. You can freely develop it yourself based on this framework.

**I will update the APP application code gradually as I sort it out.**

## 3. Visual Studio Simulator & Image Conversion Script

exist`Software`The folder contains a Visual Studio project. After opening it with VS (the C++ development component needs to be installed), the interface effect of LVGL can be simulated on the computer. After modification, the code can be pasted into the Arduino firmware to complete the interface transplantation.

> This saves the need to re-cross-compile the Arduino firmware for each modification, improving development efficiency.

![](/5.Docs/Images/Holo4.jpg)

`ImageToHolo`The folder contains a Python script for converting images into image resources used in HoloCubic firmware.

> Because image resources generally take up a lot of space, if they are all stored in the ESP32's Flash, it will not be able to save a few images. Therefore, I transplanted LVGL's FAT file system support into the framework, and the image resources can be stored in the SD card for reading.
>
> The official image conversion tool is online:<https://lvgl.io/tools/imageconverter>, need to select`Indexed 4 colors`Format.
>
> **However, the official tool can only convert one page at a time and it is very troublesome to upload and download.**, so I wrote a script for batch conversion.

The image resource used by HoloCubic is named`xxx.bin`File, you can use the script I provided to convert it and put it into the SD card, and then you can read it like this:

    lv_obj_t* imgbtn = lv_imgbtn_create(lv_scr_act(), NULL);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_PRESSED, "S:/dir/icon_pressed.bin");
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_RELEASED, "S:/dir/icon_released.bin");

in`S:`Refers to the SD card root directory (note**S is capital**), and the following is completely consistent with the path in Linux.

> This script refers to[W-Mai/lvgl_image_converter](https://github.com/W-Mai/lvgl_image_converter)realization.

**In addition, since the use of the conversion script requires a Python environment, if you do not want to install the environment, you can also use my precompiled exe file to convert. The method of use is very simple.`jpg/png/bmp`Drag the picture to`holo转换器.exe`Just click on the icon (you can drag multiple ones up at the same time), and the corresponding icons will be generated in the current directory.`.bin`document.**

> Download address of converter software:
>
> Link:<https://pan.baidu.com/s/11cPOVYnKkxmd88o-Ouwb5g>Extraction code: xlju

## 4. About the beam splitter prism

The prism I used is 25.4mm x 25.4mm x 25.4mm. You can find it on Taobao, and the price is about 80 yuan per unit.

Fixing the dichroic prism is troublesome. If you use glue, it will easily penetrate into the screen and cause watermarks. Therefore, it is recommended to search on TB.`OCA胶`, this is a`全贴合屏幕工艺`The solid glue used to bond screens is very effective and very cheap.

> However, OCA is very sticky, so you must be careful not to leave any bubbles when operating it, otherwise it will be difficult to remove once it is stuck.

## I’ll add more later, if it’s useful, remember to give me a star~
