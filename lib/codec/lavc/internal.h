#if !defined(_codec_lavc_internal_H)
# define _codec_lavc_internal_H (1)

#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/frame.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

extern AVCodec *h264;
extern AVCodec *aac;

int kr_lavc_global_init();

#endif
