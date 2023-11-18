include $(top_srcdir)/common.mk

bin_PROGRAMS = kr_quvi kr_jschema kr_json kr_sizeof kr_js_proto
bin_PROGRAMS += kr_features kr_rust_structs
#bin_PROGRAMS += kr_avprobe
#bin_PROGRAMS = kr_gif2vid
#bin_PROGRAMS = kr_dl_stream kr_v4l2_stream kr_x11_stream

kr_js_proto_SOURCES = kr_js_proto.c
kr_js_proto_CFLAGS =
kr_js_proto_LDFLAGS = ../lib/radio/libkradradio.la \
 ../lib/radio/libkradradioclient.la

kr_rust_structs_SOURCES = kr_rust_structs.c
kr_rust_structs_CFLAGS =
kr_rust_structs_LDFLAGS = ../lib/radio/libkradradio.la \
 ../lib/radio/libkradradioclient.la

#kr_dl_stream_SOURCES = kr_dl_stream.c
#kr_dl_stream_CFLAGS = $(OPUS_CFLAGS)
#kr_dl_stream_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
#kr_dl_stream_LDFLAGS = ../lib/krad_radio/libkradclient.la ../lib/krad_radio/libkradradio.la

#kr_v4l2_stream_SOURCES = kr_v4l2_stream.c
#kr_v4l2_stream_CFLAGS = $(OPUS_CFLAGS)
#kr_v4l2_stream_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
#kr_v4l2_stream_LDFLAGS = ../lib/krad_radio/libkradclient.la ../lib/krad_radio/libkradradio.la

#kr_x11_stream_SOURCES = kr_x11_stream.c
#kr_x11_stream_CFLAGS = $(OPUS_CFLAGS)
#kr_x11_stream_CFLAGS += -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
#kr_x11_stream_LDFLAGS = ../lib/krad_radio/libkradclient.la ../lib/krad_radio/libkradradio.la

#kr_gif2vid_SOURCES = kr_gif2vid.c
#kr_gif2vid_SOURCES += ../lib/image/convert.c ../lib/app/debug.c ../lib/io/file.c ../lib/mem/mem.c ../lib/image/gif.c
#kr_gif2vid_SOURCES += ../lib/image/coder.c ../lib/image/png.c ../lib/image/jpeg.c ../lib/image/image.c
#kr_gif2vid_SOURCES += ../lib/codec/video/vpx/vpx.c ../lib/time/timer.c
#kr_gif2vid_SOURCES += ../lib/ebml/pack.c ../lib/container/mkv/mkv.c
#kr_gif2vid_SOURCES += ../lib/io/stream.c ../lib/io/buffer.c
#kr_gif2vid_SOURCES += ../lib/web/transmitter.c ../lib/ring/ring.c
#kr_gif2vid_SOURCES += ../lib/hash/base64.c ../lib/loop/loop.c
#kr_gif2vid_LDFLAGS = -lgif -lswscale -lcairo -lturbojpeg $(VPX_LIBS)
#kr_gif2vid_CFLAGS = $(VPX_CFLAGS)

kr_features_SOURCES = kr_features.c
kr_features_CFLAGS =
kr_features_LDFLAGS =
kr_features_LDFLAGS += ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

kr_quvi_SOURCES = kr_quvi.c
kr_quvi_CFLAGS = $(QUVI_CFLAGS)
kr_quvi_LDFLAGS = $(QUVI_LIBS)
kr_quvi_LDFLAGS += ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

#kr_avprobe_SOURCES = kr_avprobe.c
#kr_avprobe_CFLAGS = $(CFLAGS)
#kr_avprobe_CFLAGS += "-Wno-error=switch"
#kr_avprobe_CFLAGS += "-Wno-error=parentheses"
#kr_avprobe_CFLAGS += $(AVFORMAT_CFLAGS) $(AVCODEC_CFLAGS) $(AVRESAMPLE_CFLAGS) $(AVUTIL_CFLAGS) $(AVDEVICE_CFLAGS) $(AVFILTER_CFLAGS) $(LIBSWSCALE_CFLAGS)
#kr_avprobe_LDFLAGS = $(AVFORMAT_LIBS) $(AVCODEC_LIBS) $(AVRESAMPLE_LIBS) $(AVFILTER_LIBS) $(AVUTIL_LIBS) $(AVDEVICE_LIBS) $(LIBSWSCALE_LIBS)
#kr_avprobe_LDFLAGS += "-lm"

kr_jschema_SOURCES = json/pretty.c kr_jschema.c
kr_jschema_CFLAGS = -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_jschema_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

kr_json_SOURCES = json/pretty.c kr_json.c
kr_json_CFLAGS = -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_json_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

kr_sizeof_SOURCES = kr_sizeof.c
kr_sizeof_CFLAGS = -DKR_GIT_VERSION=\"$(KR_GIT_VERSION)\"
kr_sizeof_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la
