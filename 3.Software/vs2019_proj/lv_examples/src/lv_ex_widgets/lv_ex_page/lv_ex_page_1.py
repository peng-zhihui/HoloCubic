# Create a scroll bar style
style_sb = lv.style_t()
lv.style_copy(style_sb, lv.style_plain)
style_sb.body.main_color = lv.color_make(0,0,0)
style_sb.body.grad_color = lv.color_make(0,0,0)
style_sb.body.border.color = lv.color_make(0xff,0xff,0xff)
style_sb.body.border.width = 1
style_sb.body.border.opa = lv.OPA._70
style_sb.body.radius = 800 # large enough to make a circle
style_sb.body.opa = lv.OPA._60
style_sb.body.padding.right = 3
style_sb.body.padding.bottom = 3
style_sb.body.padding.inner = 8        # Scrollbar width

# Create a page
page = lv.page(lv.scr_act())
page.set_size(150, 200)
page.align(None, lv.ALIGN.CENTER, 0, 0)
page.set_style(lv.page.STYLE.SB, style_sb)           # Set the scrollbar style

# Create a label on the page
label = lv.label(page)
label.set_long_mode(lv.label.LONG.BREAK)       # Automatically break long lines
label.set_width(page.get_fit_width())          # Set the label width to max value to not show hor. scroll bars
label.set_text("""Lorem ipsum dolor sit amet, consectetur adipiscing elit,
sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
Ut enim ad minim veniam, quis nostrud exercitation ullamco
laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure
dolor in reprehenderit in voluptate velit esse cillum dolore
eu fugiat nulla pariatur.
Excepteur sint occaecat cupidatat non proident, sunt in culpa
qui officia deserunt mollit anim id est laborum.""")
