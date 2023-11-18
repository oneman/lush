include $(top_srcdir)/common.mk

bin_PROGRAMS = kr kr_aux
#kr_snapshot

if KR_IZ_US
noinst_PROGRAMS = kr_client_example kr_test_client kr_videoport_example
noinst_PROGRAMS += kr_mixer_connect kr_aux_test kr_aux_stdin
endif

kr_SOURCES = kr.c
kr_CFLAGS =
kr_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la

kr_aux_SOURCES = kr_aux.c
kr_aux_CFLAGS = $(CAIRO_CFLAGS)
kr_aux_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_aux_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la $(CAIRO_LIBS)

kr_aux_test_SOURCES = kr_aux_test.c
kr_aux_test_CFLAGS = $(CAIRO_CFLAGS)
kr_aux_test_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_aux_test_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la $(CAIRO_LIBS)

kr_aux_stdin_SOURCES = kr_aux_stdin.c
kr_aux_stdin_CFLAGS = $(CAIRO_CFLAGS)
kr_aux_stdin_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_aux_stdin_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la $(CAIRO_LIBS)

#kr_snapshot_SOURCES = kr_snapshot.c
#kr_snapshot_CFLAGS = $(CAIRO_CFLAGS)
#kr_snapshot_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
#kr_snapshot_LDFLAGS = ../lib/krad_radio/libkradclient.la $(CAIRO_LIBS)

kr_client_example_SOURCES = kr_client_example.c
kr_client_example_CFLAGS =
kr_client_example_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_client_example_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la

kr_test_client_SOURCES = kr_test_client.c
kr_test_client_CFLAGS =
kr_test_client_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_test_client_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la

kr_videoport_example_SOURCES = kr_videoport_example.c
kr_videoport_example_CFLAGS = $(CAIRO_CFLAGS)
kr_videoport_example_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_videoport_example_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la $(CAIRO_LIBS)

kr_mixer_connect_SOURCES = kr_mixer_connect.c
kr_mixer_connect_CFLAGS =
kr_mixer_connect_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_mixer_connect_LDFLAGS = ../lib/radio/libkradradioclient.la ../lib/radio/libkradradio.la
