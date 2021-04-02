cont = lv.cont(lv.scr_act())
cont.set_auto_realign(True)                    # Auto realign when the size changes
cont.align_origo(None, lv.ALIGN.CENTER, 0, 0)  # This parametrs will be sued when realigned
cont.set_fit(lv.FIT.TIGHT)
cont.set_layout(lv.LAYOUT.COL_M)

label = lv.label(cont)
label.set_text("Short text")

label = lv.label(cont)
label.set_text("It is a long text")

label = lv.label(cont)
label.set_text("Here is an even longer text")