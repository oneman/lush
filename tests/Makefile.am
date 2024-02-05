include $(top_srcdir)/common.mk

noinst_PROGRAMS = cbor_test pool_test gif_test image_convert_test image_place_test image_sub_test
noinst_PROGRAMS += graph_test graph_tunits path_test json_to_patchset_test compositor_test jschema_test
noinst_PROGRAMS += thread_test crypto_test xcb_test url_grabber_test loop_test kr_path_test kr_diff_test
noinst_PROGRAMS += dtls_srtp_test utf8_test web_server_test web_proxy_test scan_test
noinst_PROGRAMS += web_client_test broadcast_test dir_test map_test onepole_test seq_test
noinst_PROGRAMS += xfer_test cons_test m3u_test mts_test ogg_test ogg_mux_test mkv_demux_test tracker_test y4m_test
noinst_PROGRAMS += theora_enc_test h264_enc_test vpx_enc_test y4m_enc_test mp4_demux_test
noinst_PROGRAMS += oggflac_enc_test flac_enc_test image_color_test audio_test hids_test
noinst_PROGRAMS += mkv_mux_test slider_test fader_test mp3_enc_test aac_enc_test meta_loop_test mts_mux_test
noinst_PROGRAMS += wave_test txt_mux_test bitbuffer_test mp3_dec_test aac_dec_test vorbis_dec_test opus_dec_test flac_dec_test
noinst_PROGRAMS += flac_demux_test gzip_test id3_parse_test mp3_parse_test aac_parse_test
noinst_PROGRAMS += uring_test unspecified_test

if HAVE_DAALA
noinst_PROGRAMS += daala_enc_test
endif

if HAVE_XMMS2
AM_CPPFLAGS += -DKR_XMMS2
endif

if HAVE_GTK
AM_CPPFLAGS += -DKR_USE_GTK
endif

aac_parse_test_SOURCES = container/aac_parse_test.c \
  ../lib/app/spinlock.c \
  ../lib/app/debug.c \
  ../lib/mem/mem.c \
  ../lib/container/mpeg/adts/parse.c
aac_parse_test_LDFLAGS = -lm

mp3_parse_test_SOURCES = container/mp3_parse_test.c \
  ../lib/app/spinlock.c \
  ../lib/app/debug.c \
  ../lib/mem/mem.c \
  ../lib/container/mpeg/mpga/id3.c \
  ../lib/container/mpeg/mpga/parse.c
mp3_parse_test_LDFLAGS = -lm

id3_parse_test_SOURCES = container/id3_parse_test.c \
  ../lib/app/spinlock.c \
  ../lib/app/debug.c \
  ../lib/mem/mem.c \
  ../lib/container/mpeg/mpga/id3.c
id3_parse_test_LDFLAGS = -lm

gzip_test_SOURCES = gzip_test.c \
  ../lib/app/spinlock.c \
  ../lib/app/debug.c \
  ../lib/mem/mem.c \
  ../lib/web/encoding/gzip.c
gzip_test_LDFLAGS = -lm -lz

flac_demux_test_SOURCES = container/flac_demux_test.c
flac_demux_test_SOURCES += ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/container/flac/unpack.c
flac_demux_test_LDFLAGS = -lm

flac_dec_test_SOURCES = codec/flac_dec_test.c
flac_dec_test_SOURCES += ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/convert.c \
  ../lib/container/wave/pack.c \
  ../lib/container/flac/unpack.c \
  ../lib/codec/audio/flac/decoder.c
flac_dec_test_LDFLAGS = -lFLAC -lm

vorbis_dec_test_SOURCES = codec/vorbis_dec_test.c
vorbis_dec_test_SOURCES += ../lib/container/wave/pack.c
vorbis_dec_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

opus_dec_test_SOURCES = codec/opus_dec_test.c
opus_dec_test_SOURCES += ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/convert.c \
  ../lib/container/wave/pack.c \
  ../lib/container/ogg/parse.c \
  ../lib/container/ogg/unpack.c \
  ../lib/codec/audio/opus/decoder.c
opus_dec_test_LDFLAGS = -lopus -lm

aac_dec_test_SOURCES = codec/aac_dec_test.c
aac_dec_test_SOURCES += ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/convert.c \
  ../lib/container/wave/pack.c \
  ../lib/codec/audio/aac/decoder.c
aac_dec_test_LDFLAGS = -lfdk-aac -lm

mp3_dec_test_SOURCES = codec/mp3_dec_test.c
mp3_dec_test_SOURCES += ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/convert.c \
  ../lib/audio/resample.c \
  ../lib/container/wave/pack.c \
  ../lib/codec/audio/mp3/decoder.c
mp3_dec_test_LDFLAGS = -lmp3lame -lsamplerate -lm

bitbuffer_test_SOURCES = bitbuffer_test.c
bitbuffer_test_SOURCES +=
bitbuffer_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

txt_mux_test_SOURCES = container/txt_mux_test.c
txt_mux_test_SOURCES += ../lib/mem/mem.c \
../lib/app/debug.c \
../lib/app/spinlock.c \
../lib/ebml/pack.c \
../lib/container/txt/pack.c
txt_mux_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

wave_test_SOURCES = container/wave_test.c
wave_test_SOURCES += ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/container/wave/pack.c \
  ../lib/container/wave/unpack.c
wave_test_LDFLAGS = -lm

web_proxy_test_SOURCES = web_proxy_test.c
web_proxy_test_CFLAGS =
web_proxy_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

mts_mux_test_SOURCES = container/mts_mux_test.c
mts_mux_test_SOURCES += ../lib/mem/mem.c \
../lib/app/debug.c \
../lib/app/spinlock.c \
../lib/audio/convert.c \
../lib/container/mpeg/ts/pack.c \
../lib/container/wave/unpack.c \
../lib/codec/audio/aac/encoder.c
mts_mux_test_LDFLAGS = -lm -lfdk-aac
mts_mux_test_LDFLAGS += ../lib/radio/libkradradio.la

meta_loop_test_SOURCES = util/meta_loop_test.c
meta_loop_test_CFLAGS =
meta_loop_test_LDFLAGS = ../lib/radio/libkradradio.la \
 ../lib/radio/libkradradioclient.la

aac_enc_test_SOURCES = codec/aac_enc_test.c
aac_enc_test_SOURCES += ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/noise.c \
  ../lib/audio/convert.c \
  util/tone.c \
  ../lib/container/mpeg/ts/pack.c \
  ../lib/codec/audio/aac/encoder.c
aac_enc_test_LDFLAGS = -lfdk-aac -lm

mp3_enc_test_SOURCES = codec/mp3_enc_test.c
mp3_enc_test_SOURCES += ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/noise.c \
  ../lib/audio/convert.c \
  util/tone.c \
  ../lib/codec/audio/mp3/encoder.c
mp3_enc_test_LDFLAGS = -lmp3lame -lm

xfer_test_SOURCES = xfer/xfer_test.c
xfer_test_CFLAGS =
xfer_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

mkv_mux_test_SOURCES = container/mkv_mux_test.c
mkv_mux_test_SOURCES += ../lib/mem/mem.c \
../lib/app/debug.c \
../lib/app/spinlock.c \
../lib/ebml/pack.c \
../lib/container/mkv/pack.c
mkv_mux_test_LDFLAGS = -lm

hids_test_SOURCES = hash/hids_test.c
hids_test_SOURCES += ../lib/hash/hashids.c \
  ../lib/mem/mem.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c

fader_test_SOURCES = fader_test.c
fader_test_SOURCES +=   ../lib/image/image.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/image/coder.c \
	../lib/draw/blur.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c
fader_test_LDFLAGS = -lm -lswscale -lcairo -lturbojpeg

image_color_test_SOURCES = image/image_color_test.c
image_color_test_SOURCES +=   ../lib/image/image.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/image/coder.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  util/image.c
image_color_test_LDFLAGS = -lswscale -lcairo -lturbojpeg

vpx_enc_test_SOURCES = codec/vpx_enc_test.c
vpx_enc_test_SOURCES += ../lib/image/convert.c \
  ../lib/image/coder.c \
  ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/image/image.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/codec/video/vpx/encoder.c \
  util/image.c
vpx_enc_test_LDFLAGS = -lvpx -lswscale -lcairo -lturbojpeg ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

h264_enc_test_SOURCES = codec/h264_enc_test.c
h264_enc_test_SOURCES += ../lib/image/convert.c \
  ../lib/image/image.c \
  ../lib/image/coder.c \
  ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/codec/video/h264/encoder.c \
  util/image.c
h264_enc_test_LDFLAGS = -lx264 -lswscale -lcairo -lturbojpeg
h264_enc_test_LDFLAGS += ../lib/radio/libkradradio.la

flac_enc_test_SOURCES = codec/flac_enc_test.c
flac_enc_test_LDFLAGS = ../lib/radio/libkradradio.la

audio_test_SOURCES = audio_test.c
audio_test_SOURCES += ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/audio.c \
  ../lib/audio/noise.c \
  ../lib/audio/convert.c \
  util/tone.c \
  ../lib/codec/audio/flac/encoder.c
audio_test_LDFLAGS = -lFLAC -lm

slider_test_SOURCES = slider_test.c
slider_test_SOURCES += ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/audio.c
slider_test_LDFLAGS = -lm

oggflac_enc_test_SOURCES = codec/oggflac_enc_test.c
oggflac_enc_test_SOURCES += ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/audio/noise.c \
  util/tone.c \
  ../lib/audio/convert.c \
  ../lib/container/ogg/pack.c \
  ../lib/codec/audio/flac/encoder.c
oggflac_enc_test_LDFLAGS = -lFLAC -logg -lm

y4m_enc_test_SOURCES = codec/y4m_enc_test.c
y4m_enc_test_SOURCES += ../lib/image/convert.c \
  ../lib/image/image.c \
  ../lib/image/coder.c \
  ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/container/y4m/pack.c \
  ../lib/codec/video/y4m/encoder.c \
  util/image.c
y4m_enc_test_LDFLAGS = -lswscale -lcairo -lturbojpeg
y4m_enc_test_LDFLAGS += ../lib/radio/libkradradio.la

if HAVE_DAALA
daala_enc_test_SOURCES = codec/daala_enc_test.c
daala_enc_test_SOURCES += ../lib/image/convert.c \
  ../lib/image/image.c \
  ../lib/image/coder.c \
  ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/container/ogg/pack.c \
  ../lib/codec/video/daala/encoder.c \
  util/image.c
daala_enc_test_LDFLAGS = -ldaalabase -ldaalaenc -logg -lswscale -lcairo -lturbojpeg
daala_enc_test_LDFLAGS += ../lib/radio/libkradradio.la
endif

theora_enc_test_SOURCES = codec/theora_enc_test.c
theora_enc_test_SOURCES += ../lib/image/convert.c \
  ../lib/image/image.c \
  ../lib/image/coder.c \
  ../lib/io/file.c \
  ../lib/app/debug.c \
  ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/image/png.c \
  ../lib/image/jpeg.c \
  ../lib/container/ogg/pack.c \
  ../lib/codec/video/theora/encoder.c \
  util/image.c
theora_enc_test_LDFLAGS = -ltheoraenc -ltheoradec -logg -lswscale -lcairo -lturbojpeg
theora_enc_test_LDFLAGS += ../lib/radio/libkradradio.la

seq_test_SOURCES = seq_test.c
seq_test_SOURCES += ../lib/mem/mem.c \
../lib/mem/pool.c \
../lib/loop/loop.c \
../lib/app/debug.c \
../lib/app/seq.c \
../lib/app/spinlock.c
seq_test_LDFLAGS = -lm ../lib/radio/libkradradio.la

tracker_test_SOURCES = container/tracker_test.c
tracker_test_SOURCES += ../lib/mem/mem.c \
../lib/mem/pool.c \
../lib/loop/loop.c \
../lib/app/debug.c \
../lib/app/seq.c \
../lib/app/spinlock.c \
../lib/container/ogg/pack.c \
../lib/container/ogg/unpack.c \
../lib/container/ogg/parse.c \
../lib/hash/hashids.c \
../lib/track/tracker.c \
../lib/system/priority.c
tracker_test_LDFLAGS = -lm

ogg_test_SOURCES = container/ogg_test.c
ogg_test_SOURCES += ../lib/mem/mem.c \
../lib/app/debug.c \
../lib/app/spinlock.c \
../lib/container/ogg/unpack.c \
../lib/container/ogg/pack.c \
../lib/container/ogg/parse.c

ogg_mux_test_SOURCES = container/ogg_mux_test.c
ogg_mux_test_SOURCES += ../lib/mem/mem.c \
../lib/app/debug.c \
../lib/app/spinlock.c \
../lib/container/ogg/pack.c

mkv_demux_test_SOURCES = container/mkv_demux_test.c
mkv_demux_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

mp4_demux_test_SOURCES = container/mp4_demux_test.c
mp4_demux_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

mts_test_SOURCES = container/mts_test.c util/image.c
mts_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

m3u_test_SOURCES = playlist/m3u_test.c \
 ../lib/playlist/m3u.c \
 ../lib/tuner/hls_tuner.c \
 ../lib/app/debug.c \
 ../lib/app/spinlock.c \
 ../lib/mem/mem.c \
 ../lib/io/file.c
m3u_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

y4m_test_SOURCES = container/y4m_test.c
y4m_test_CFLAGS =
y4m_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

onepole_test_SOURCES = onepole.c
onepole_test_CFLAGS =
onepole_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

cons_test_SOURCES = util/consistency_test.c
cons_test_CFLAGS =
cons_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

map_test_SOURCES = map_test.c
map_test_SOURCES += ../lib/mem/mem.c ../lib/mem/pool.c  ../lib/app/debug.c ../lib/app/spinlock.c \
../lib/map/map.c ../lib/time/timer.c \
 ../lib/hash/murmur3.c
map_test_CFLAGS =
map_test_LDFLAGS = -lm -luuid

dir_test_SOURCES = io/dir_test.c
dir_test_SOURCES += ../lib/mem/mem.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/io/dir.c
dir_test_CFLAGS =
dir_test_LDFLAGS =

scan_test_SOURCES = io/scan_test.c
scan_test_SOURCES += ../lib/mem/mem.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/io/dir.c
scan_test_CFLAGS = $(XMMS2_CFLAGS)
scan_test_LDFLAGS = $(XMMS2_LIBS)

broadcast_test_SOURCES = broadcast_test.c
broadcast_test_SOURCES += ../lib/mem/pool.c ../lib/app/debug.c \ ../lib/app/spinlock.c ../lib/app/broadcast.c
broadcast_test_SOURCES += ../lib/mem/mem.c
broadcast_test_CFLAGS =
broadcast_test_LDFLAGS =

web_client_test_SOURCES = web_client_test.c
web_client_test_CFLAGS =
web_client_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

web_server_test_SOURCES = web_server_test.c
web_server_test_CFLAGS =
web_server_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

kr_diff_test_SOURCES = util/kr_diff_test.c ../tools/json/pretty.c
kr_diff_test_LDFLAGS = ../lib/radio/libkradradio.la ../lib/radio/libkradradioclient.la

kr_path_test_SOURCES = util/kr_path_test.c ../lib/app/path.c ../lib/app/debug.c \ ../lib/app/spinlock.c

utf8_test_SOURCES = utf8_test.c
utf8_test_CFLAGS =
utf8_test_LDFLAGS =

jschema_test_SOURCES = json/jschema_test.c
jschema_test_LDFLAGS = ../lib/radio/libkradradio.la

compositor_test_SOURCES = compositor/compositor_test.c util/image.c
compositor_test_CFLAGS = $(CAIRO_CFLAGS) $(GTK_CFLAGS)
compositor_test_LDFLAGS = ../lib/radio/libkradradio.la
compositor_test_LDFLAGS += $(CAIRO_LIBS) $(GTK_LIBS)

loop_test_SOURCES = loop/loop_test.c
loop_test_CFLAGS =
loop_test_LDFLAGS = ../lib/radio/libkradradio.la

gif_test_SOURCES = image/gif_test.c
gif_test_SOURCES += ../lib/image/convert.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/io/file.c ../lib/mem/mem.c ../lib/video/gif.c
gif_test_SOURCES += ../lib/image/coder.c ../lib/image/png.c ../lib/image/jpeg.c util/image.c ../lib/image/image.c
gif_test_CFLAGS =
gif_test_LDFLAGS = -lgif -lswscale -lcairo -lturbojpeg
gif_test_LDFLAGS += ../lib/radio/libkradradio.la

image_convert_test_SOURCES = image/convert_test.c
image_convert_test_SOURCES += ../lib/image/convert.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/io/file.c ../lib/mem/mem.c ../lib/video/gif.c
image_convert_test_SOURCES += ../lib/image/coder.c ../lib/image/png.c ../lib/image/jpeg.c util/image.c ../lib/image/image.c
image_convert_test_CFLAGS =
image_convert_test_LDFLAGS = -lgif -lswscale -lcairo -lturbojpeg
image_convert_test_LDFLAGS += ../lib/radio/libkradradio.la

image_place_test_SOURCES = image/place_test.c
image_place_test_SOURCES += ../lib/image/convert.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/io/file.c ../lib/mem/mem.c ../lib/video/gif.c ../lib/image/image.c
image_place_test_SOURCES += ../lib/image/coder.c ../lib/image/png.c ../lib/image/jpeg.c ../lib/image/crop.c ../lib/image/insert.c util/image.c
image_place_test_CFLAGS = $(CAIRO_CFLAGS)
image_place_test_LDFLAGS = -lgif -lswscale -lcairo -lturbojpeg -lm
image_place_test_LDFLAGS += ../lib/radio/libkradradio.la

image_sub_test_SOURCES = image/sub_test.c
image_sub_test_SOURCES += ../lib/image/convert.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/io/file.c ../lib/mem/mem.c ../lib/video/gif.c ../lib/image/image.c
image_sub_test_SOURCES += ../lib/image/coder.c ../lib/image/png.c ../lib/image/jpeg.c ../lib/image/crop.c util/image.c
image_sub_test_CFLAGS =
image_sub_test_LDFLAGS = -lgif -lswscale -lcairo -lturbojpeg
image_sub_test_LDFLAGS += ../lib/radio/libkradradio.la

json_to_patchset_test_SOURCES = json/json_to_patchset.c ../lib/json/parser.c
json_to_patchset_test_CFLAGS =
json_to_patchset_test_LDFLAGS = ../lib/radio/libkradradio.la

crypto_test_SOURCES = crypto/crypto_test.c ../lib/app/debug.c ../lib/app/spinlock.c
crypto_test_CFLAGS =
crypto_test_LDFLAGS = -lssl -lcrypto

dtls_srtp_test_SOURCES = crypto/dtls_srtp_test.c ../lib/app/debug.c ../lib/app/spinlock.c ../lib/web/protocol/dtls-srtp.c
dtls_srtp_test_CFLAGS =
dtls_srtp_test_LDFLAGS = -lssl -lcrypto

thread_test_SOURCES = krad/thread_test.c
thread_test_CFLAGS =
thread_test_LDFLAGS =

url_grabber_test_SOURCES = io/url_grabber_test.c \
  ../lib/io/stream.c \
  ../lib/io/file.c \
  ../lib/mem/mem.c \
  ../lib/hash/base64.c \
  ../lib/time/timer.c \
  ../lib/app/debug.c ../lib/app/spinlock.c

url_grabber_test_LDFLAGS = -lm

path_test_SOURCES = krad/path_test.c
path_test_SOURCES += ../lib/app/path.c ../lib/mem/mem.c ../lib/app/debug.c ../lib/app/spinlock.c
path_test_CFLAGS =
path_test_LDFLAGS =

cbor_test_SOURCES = cbor/cbor_test.c
cbor_test_SOURCES += ../lib/cbor/cbor.c ../lib/mem/mem.c ../lib/app/debug.c ../lib/app/spinlock.c
cbor_test_CFLAGS =
cbor_test_LDFLAGS =

pool_test_SOURCES = mem/pool_test.c
pool_test_SOURCES += ../lib/mem/pool.c ../lib/mem/mem.c ../lib/app/debug.c ../lib/app/spinlock.c
pool_test_CFLAGS =
pool_test_LDFLAGS =

xcb_test_SOURCES = x11/xcb_test.c
xcb_test_SOURCES += ../lib/app/debug.c ../lib/app/spinlock.c
xcb_test_CFLAGS = $(X11_CFLAGS)
xcb_test_LDFLAGS = $(X11_LIBS)

graph_test_SOURCES = ../lib/app/debug.c ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/time/timer.c \
  graph/graph_test.c
graph_test_LDFLAGS = -lm

graph_tunits_SOURCES = ../lib/app/debug.c ../lib/app/spinlock.c \
  ../lib/mem/mem.c \
  ../lib/time/timer.c \
  ../lib/graph/graph.c \
   graph/graph_tunits.c
graph_tunits_CFLAGS = -g
graph_tunits_LDFLAGS = -lm

uring_test_SOURCES = ../lib/app/debug.c\
  uring_test.c
uring_test_CFLAGS =
uring_test_LDFLAGS = -lm -luring
uring_test_LDFLAGS += ../lib/radio/libkradradio.la

unspecified_test_SOURCES = ../lib/app/debug.c\
  unspecified_test.c
unspecified_test_CFLAGS =
unspecified_test_LDFLAGS = -lm -luring
unspecified_test_LDFLAGS += ../lib/radio/libkradradio.la

.PHONY: all
