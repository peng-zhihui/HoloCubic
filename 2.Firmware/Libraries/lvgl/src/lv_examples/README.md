# Examples for LVGL

Here you can find examples and demos for LVGL embedded GUI library. 


## Add the examples to your projects
1. Clone this repository: `git clone https://github.com/lvgl/lv_examples.git`.
2. The `lv_examples` directory should be next to the `lvgl` directory in your project.

Similarly to `lv_conf.h` there is a configuration file for the examples too. It is called `lv_ex_conf.h`.
1. Copy `lv_examples/lv_ex_conf_templ.h` next to `lv_examples` directory
2. Rename it to `lv_ex_conf.h`
3. Change the first `#if 0` to `#if 1` to enable the file's content
4. Enable or Disable modules

## Examples

### Get strated
If you are new to LVGL check these examples first. They demonstrate the basic mechanisms of the library.
See the [lv_ex_get_started](https://github.com/lvgl/lv_examples/tree/master/src/lv_ex_get_started) folder.
You can also take a look at the [Quich overview](https://docs.lvgl.io/v7/en/html/get-started/quick-overview.html) of LVGL to learn the basics. 

### Styles
LVGL has a powerful and versatile style system. These examples show how to use the most common style properties, e.g. styling rectangles, shadows, images, lines, transitions and so on. See the [lv_ex_style](https://github.com/lvgl/lv_examples/tree/master/src/lv_ex_style) folder.
Be sure to read the [Documentation](https://docs.lvgl.io/v7/en/html/overview/style.html) of the styles to fully understand them.

### Widgets
LVGL has more than 35 widgets and you can find examples for all of them.
Go to the [lv_ex_widgets](https://github.com/lvgl/lv_examples/tree/master/src/lv_ex_widgets) folder to see them.
The documentation of the widgets can be found [here](https://docs.lvgl.io/v7/en/html/widgets/index.html).

## Demos

### Widgets
Shows how the widgets look like out of the box using the built-in material theme.  
See in [lv_demo_widgets](https://github.com/lvgl/lv_examples/tree/master/src/lv_demo_widgets) folder.

<img src="https://github.com/lvgl/lv_examples/blob/master/src/lv_demo_widgets/screenshot1.png?raw=true" width=600px alt="Basic demo to show the widgets of LVGL">

### Printer
A printer example created with LVGL. This demo is optimized for 800x480 resolution and among many others, it demonstrates custom theme creation, style transitions, and animations. 
See in [lv_demo_printer](https://github.com/lvgl/lv_examples/tree/master/src/lv_demo_printer) folder.
<img src="https://github.com/lvgl/lv_examples/blob/master/src/lv_demo_printer/screenshot1.png?raw=true" width=600px alt="Printer demo with LVGL embedded GUI library">

### Keypad and encoder
LVGL allows you to control the widgets with a keypad and/or encoder without a touchpad. This demo shows how to handle buttons, drop-down lists, rollers, sliders, switches, and text inputs without touchpad. 
Learn more about the touchpad-less usage of LVGL [here](https://docs.lvgl.io/v7/en/html/overview/indev.html#keypad-and-encoder).

See in [lv_demo_keypad_encoder](https://github.com/lvgl/lv_examples/tree/master/src/lv_demo_keypad_encoder) folder.

<img src="https://github.com/lvgl/lv_examples/blob/master/src/lv_demo_keypad_encoder/screenshot1.png?raw=true" width=600px alt="Keypad and encoder navigation in LVGL embedded GUI library">

### Benchmark
A demo to measure the performance of LVGL or to compare different settings. 
See in [lv_demo_benchmark](https://github.com/lvgl/lv_examples/tree/master/src/lv_demo_benchmark) folder.
<img src="https://github.com/lvgl/lv_examples/blob/master/src/lv_demo_benchmark/screenshot1.png?raw=true" width=600px alt="Benchmark demo with LVGL embedded GUI library">

### Stress
A stress test for LVGL. It contains a lot of object creation, deletion, animations, style usage, and so on. It can be used if there is any memory corruption during heavy usage or any memory leaks. 
See in [lv_demo_stress](https://github.com/lvgl/lv_examples/tree/master/src/lv_demo_stress) folder.
<img src="https://github.com/lvgl/lv_examples/blob/master/src/lv_demo_stress/screenshot1.png?raw=true" width=600px alt="Stress tes tfor LVGL">

## Contributing
For contribution and coding style guidelines, please refer to the file docs/CONTRIBUTNG.md in the main LVGL repo:
  https://github.com/lvgl/lvgl
