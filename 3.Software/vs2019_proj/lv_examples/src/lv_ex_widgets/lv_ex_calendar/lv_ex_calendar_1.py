def event_handler(obj, event):
    if event == lv.EVENT.CLICKED:
        date = obj.get_pressed_date()
        if date is not None:
            obj.set_today_date(date)

calendar = lv.calendar(lv.scr_act())
calendar.set_size(230, 230)
calendar.align(None, lv.ALIGN.CENTER, 0, 0)
calendar.set_event_cb(event_handler)

# Set today's date
today = lv.calendar_date_t()
today.year = 2018
today.month = 10
today.day = 23

calendar.set_today_date(today)
calendar.set_showed_date(today)

highlihted_days = [
    lv.calendar_date_t({'year':2018, 'month':10, 'day':6}),
    lv.calendar_date_t({'year':2018, 'month':10, 'day':11}),
    lv.calendar_date_t({'year':2018, 'month':11, 'day':22})
]

calendar.set_highlighted_dates(highlihted_days, len(highlihted_days))