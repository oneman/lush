include $(top_srcdir)/common.mk

lib_LTLIBRARIES = radio/libkradradioclient.la radio/libkradradio.la

radio_libkradradio_la_CXXFLAGS =
radio_libkradradio_la_CFLAGS = $(XKBCOMMON_CFLAGS) $(X11_CFLAGS)\
 $(CAIRO_CFLAGS) $(XMMS2_CFLAGS) $(OPUS_CFLAGS) -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
radio_libkradradio_la_CFLAGS += $(WLROOTS_CFLAGS)
radio_libkradradio_la_CFLAGS += -D_GNU_SOURCE=1
radio_libkradradio_la_CFLAGS += -I/usr/include/mupdf/fitz
radio_libkradradio_la_LIBADD = radio/libkradradioclient.la

radio_libkradradio_la_LIBADD += $(CAIRO_LIBS) $(FLAC_LIBS) $(JACK_LIBS)\
 $(VORBIS_LIBS)	$(XMMS2_LIBS) $(THEORA_LIBS) $(VPX_LIBS) $(X264_LIBS)\
 $(AVUTIL_LIBS) $(AVFORMAT_LIBS) $(AVCODEC_LIBS) $(LIBSWSCALE_LIBS)\
 $(WLROOTS_LIBS)\
 $(X11_LIBS) $(SAMPLERATE_LIBS) $(ALSA_LIBS) $(WAYLAND_LIBS) $(XKBCOMMON_LIBS)\
 $(LIBUDEV_LIBS) $(OPUS_LIBS) $(TLS_LIBS) $(DAALA_LIBS) $(AAC_LIBS) -lpthread

radio_libkradradio_la_LDFLAGS = -shared -no-undefined \
 -version-info @KR_LT_CURRENT@:@KR_LT_REVISION@:@KR_LT_AGE@
radio_libkradradio_la_LDFLAGS += -lmupdf
radio_libkradradio_la_SOURCES = \
cnake/code.c\
adapter/sw/aux/aux.c \
app/server.c \
app/queue.c \
audio/audio.c \
audio/convert.c \
audio/debug.c \
audio/signal.c \
audio/eq.c \
audio/onepole.c \
audio/volume.c \
audio/analog.c \
audio/pass.c \
audio/hardlimiter.c \
audio/afx.c \
audio/meter.c \
audio/noise.c \
audio/pool.c \
audio/resample.c \
codec/lavc/internal.c \
codec/lavc/decoder.c \
codec/lavc/encoder.c \
codec/audio/flac/encoder.c \
codec/audio/flac/decoder.c \
codec/audio/vorbis/encoder.c \
codec/audio/vorbis/decoder.c \
codec/audio/opus/encoder.c \
codec/audio/opus/decoder.c \
codec/audio/opus/header.c \
codec/video/y4m/encoder.c \
codec/video/theora/encoder.c \
codec/video/h264/encoder.c \
codec/video/vpx/encoder.c \
codec/audio/mp3/encoder.c \
codec/audio/mp3/decoder.c \
codec/audio/aac/encoder.c \
codec/audio/aac/decoder.c \
compositor/compositor.c \
container/flac/pack.c \
container/flac/unpack.c \
container/mpeg/ts/pack.c \
container/mpeg/ts/unpack.c \
container/mpeg/mpga/id3.c \
container/mpeg/mpga/parse.c \
container/mpeg/adts/parse.c \
container/ogg/pack.c \
container/ogg/parse.c \
container/ogg/unpack.c \
container/txt/pack.c \
container/y4m/pack.c \
container/y4m/unpack.c \
container/mkv/pack.c \
container/mkv/unpack.c \
container/occ/occ.c \
draw/blur.c \
draw/vector.c \
draw/text.c \
image/convert.c \
image/coder.c \
image/crop.c \
image/debug.c \
image/insert.c \
image/png.c \
image/jpeg.c \
image/stash.c \
loop/loop.c \
ops/server.c \
mixer/mixer.c \
perspective/perspective.c \
radio/radio.c \
ring/ring.c \
system/fdset.c \
system/cmdr.c \
system/priority.c \
tags/tags.c \
time/easing.c \
time/interval.c \
track/tracker.c \
track/studio.c \
tuner/hls_tuner.c \
video/frame.c \
web/client.c \
web/crate.c \
web/encoding/gzip.c \
web/protocol/dtls-srtp.c \
web/http/http.c \
web/protocol/osc.c \
web/protocol/tls.c \
web/protocol/websocket.c \
web/server.c \
proxy/proxy.c \
web/web.c \
xfer/xfer.c \
xpdr/xpdr.c

if HAVE_DAALA
radio_libkradradio_la_SOURCES += codec/video/daala/encoder.c
AM_CPPFLAGS += -DKR_USE_DAALA
endif

if HAVE_JACK
radio_libkradradio_la_SOURCES += adapter/sw/jack/jack.c
AM_CPPFLAGS += -DKR_USE_JACK
endif

if HAVE_XMMS2
radio_libkradradio_la_SOURCES += xmms2/xmms2.c
AM_CPPFLAGS += -DKR_XMMS2
endif

if HAVE_LIBUDEV
radio_libkradradio_la_SOURCES += adapter/monitor.c
AM_CPPFLAGS += -DKR_LIBUDEV
endif

if HAVE_WAYLAND
radio_libkradradio_la_SOURCES += adapter/sw/wayland/wayland.c
AM_CPPFLAGS += -DKR_USE_WAYLAND
endif

if HAVE_X11
radio_libkradradio_la_SOURCES += adapter/sw/x11/x11.c
AM_CPPFLAGS += -DKR_USE_X11
endif

if HAVE_TURBOJPEG
radio_libkradradio_la_LIBADD += -lturbojpeg
AM_CPPFLAGS += -DKR_TURBOJPEG
endif

if HAVE_GIF
radio_libkradradio_la_LIBADD += -lgif
AM_CPPFLAGS += -DKR_GIF
endif

if KR_DECKLINK
radio_libkradradio_la_SOURCES += adapter/hw/decklink/decklink.c \
 adapter/hw/decklink/capture.cpp \
 adapter/hw/decklink/vendor/linux/DeckLinkAPIDispatch.cpp
AM_CPPFLAGS += -DKR_USE_DECKLINK
AM_CPPFLAGS += -Iadapter/hw/decklink/vendor/linux
endif

if KR_ALSA
radio_libkradradio_la_SOURCES += adapter/hw/alsa/alsa.c
AM_CPPFLAGS += -DKR_USE_ALSA
endif

if KR_V4L2
radio_libkradradio_la_SOURCES += adapter/hw/v4l2/v4l2.c
AM_CPPFLAGS += -DKR_USE_V4L2
endif

radio_libkradradioclient_la_CFLAGS = -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
radio_libkradradioclient_la_CFLAGS += -D_GNU_SOURCE=1
radio_libkradradioclient_la_LIBADD =
radio_libkradradioclient_la_LDFLAGS = -shared -no-undefined \
 -version-info @KR_LT_CURRENT@:@KR_LT_REVISION@:@KR_LT_AGE@
radio_libkradradioclient_la_LDFLAGS += -lm
radio_libkradradioclient_la_SOURCES = \
adapter/sw/aux/client.c \
app/debug.c \
app/path.c \
app/patch.c \
app/diff.c \
app/router2.c \
app/name.c \
app/notice.c \
app/common.c \
app/crate.c \
app/client.c \
app/seq.c \
system/fdshare.c \
app/broadcast.c \
app/spinlock.c \
compositor/client.c \
ebml/pack.c \
ebml/unpack.c \
gen/info_types.c \
gen/metainfo.c \
gen/mime.c \
util/json_pack.c \
util/json_unpack.c \
util/helpers.c \
util/patch_path.c \
util/ebml_pack_unpack.c \
util/to_text.c \
util/jschema.c \
util/diff.c \
util/strton.c \
util/meta_loop.c \
gen/wrappers.c \
graph/graph.c \
hash/base64.c \
hash/sha1.c \
hash/murmur3.c \
hash/hashids.c \
json/jsmn/jsmn.c \
json/parser.c \
image/image.c \
image/pool.c \
io/buffer.c \
io/file.c \
io/file2.c \
io/dir.c \
map/map.c \
mem/mem.c \
mem/pool.c \
mixer/client.c \
radio/control.c \
radio/client.c \
time/timer.c \
time/stamp.c \
xpdr/client.c

kradradioincludedir = $(includedir)/krad
kradradioinclude_HEADERS =

.PHONY: all

install-exec-hook:
	mkdir -p $(DESTDIR)$(prefix)/share/$(PACKAGE)
	cp -r ../web $(DESTDIR)/$(prefix)/share/$(PACKAGE)/
