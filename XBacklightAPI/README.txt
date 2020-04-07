A Frankenstein API built from xbacklight.c, which can be used to set and get
the current backlight level. Calling backlight_setup() will perform the
necessary one-time setup and identify the active backlight device. Once setup,
backlight_get() will return a long int representing the current backlight
level and backlight_set(long value) will change the current backlight level to
<value>. Note that these numbers are NOT from 0 to 100, but rather from a set
minumum number to a set maximum number identified by backlight_min() and
backlight_max().  This code was written for a project that never got written,
but it took me long enough that I'd rather keep it here just in case. 

Using the API, I wrote a simple xbacklight clone which can be compiled using
the command

	gcc xbacklight_clone.c `pkg-config --libs --cflags xcb-randr xcb`
