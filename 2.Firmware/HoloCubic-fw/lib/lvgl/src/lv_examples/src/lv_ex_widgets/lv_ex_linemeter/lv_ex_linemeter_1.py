# Create a style for the line meter
style_lmeter = lv.style_t()
lv.style_copy(style_lmeter, lv.style_pretty_color)
style_lmeter.line.width = 2
style_lmeter.line.color = lv.color_hex(0xc0c0c0)              # Silver
style_lmeter.body.main_color = lv.color_hex(0x91bfed)         # Light blue
style_lmeter.body.grad_color = lv.color_hex(0x04386c)         # Dark blue
style_lmeter.body.padding.left = 16                           # Line length

# Create a line meter
lmeter = lv.lmeter(lv.scr_act())
lmeter.set_range(0, 100)                    # Set the range
lmeter.set_value(80)                        # Set the current value
lmeter.set_scale(240, 31)                   # Set the angle and number of lines
lmeter.set_style(lv.lmeter.STYLE.MAIN, style_lmeter)          # Apply the new style
lmeter.set_size(150, 150)
lmeter.align(None, lv.ALIGN.CENTER, 0, 0)