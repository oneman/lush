include $(top_srcdir)/common.mk

bin_PROGRAMS = demo

demo_SOURCES = demo.c
demo_CFLAGS =
demo_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la
demo_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
