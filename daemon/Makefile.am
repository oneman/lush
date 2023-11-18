include $(top_srcdir)/common.mk

bin_PROGRAMS = krad_radio

krad_radio_SOURCES = krad_radio.c
krad_radio_CFLAGS =
krad_radio_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la
krad_radio_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"

#krad_ops_SOURCES = krad_ops.c
#krad_ops_CFLAGS = -Iops
#krad_ops_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la
