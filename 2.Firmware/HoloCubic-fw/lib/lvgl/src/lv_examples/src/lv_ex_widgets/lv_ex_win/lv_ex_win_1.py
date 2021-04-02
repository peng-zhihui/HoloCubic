# Create a window
win = lv.win(lv.scr_act())
win.set_title("Window title")                 # Set the title


# Add control button to the header
close_btn = win.add_btn(lv.SYMBOL.CLOSE)      # Add close button and use built-in close action
close_btn.set_event_cb(lv.win.close_event_cb)
win.add_btn(lv.SYMBOL.SETTINGS)               # Add a setup button

# Add some dummy content
txt = lv.label(win)
txt.set_text(
"""This is the content of the window

You can add control buttons to
the window header

The content area becomes automatically
scrollable is it's large enough.

You can scroll the content
See the scroll bar on the right!"""
)
