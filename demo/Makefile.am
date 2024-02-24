include $(top_srcdir)/common.mk

bin_PROGRAMS = demo

demo_SOURCES = demo.c
demo_CFLAGS = $(CAIRO_CFLAGS) -I/usr/include/mupdf/fitz
demo_LDFLAGS = -lcairo -lmupdf
demo_LDFLAGS += ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la
demo_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
