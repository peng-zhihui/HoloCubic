# Create a style
style = lv.style_t()
lv.style_copy(style, lv.style_pretty_color)
style.body.main_color = lv.color_hex3(0x666)     # Line color at the beginning
style.body.grad_color =  lv.color_hex3(0x666)    # Line color at the end
style.body.padding.left = 10                     # Scale line length
style.body.padding.inner = 8                     # Scale label padding
style.body.border.color = lv.color_hex3(0x333)   # Needle middle circle color
style.line.width = 3
style.text.color = lv.color_hex3(0x333)
style.line.color = lv.color_hex3(0xF00)          # Line color after the critical value

# Describe the color for the needles
needle_colors = [
    lv.color_make(0x00, 0x00, 0xFF),
    lv.color_make(0xFF, 0xA5, 0x00),
    lv.color_make(0x80, 0x00, 0x80)
]

# Create a gauge
gauge1 = lv.gauge(lv.scr_act())
gauge1.set_style(lv.gauge.STYLE.MAIN, style)
gauge1.set_needle_count(len(needle_colors), needle_colors)
gauge1.set_size(150, 150)
gauge1.align(None, lv.ALIGN.CENTER, 0, 20)

# Set the values
gauge1.set_value(0, 10)
gauge1.set_value(1, 20)
gauge1.set_value(2, 30)