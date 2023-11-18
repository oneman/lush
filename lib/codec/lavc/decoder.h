#if !defined(_codec_avc_H)
# define _codec_avc_H (1)

typedef struct kr_audio_dec kr_audio_dec;
typedef struct kr_video_dec kr_video_dec;

int kr_avc_decode_audio(kr_audio_dec *dec, uint8_t *data, size_t sz);
int kr_avc_decode_video(kr_video_dec *dec, kr_frame *frame,
 kr_coded_image *img);
kr_audio_dec *kr_avc_audio_decoder_create();
int kr_avc_audio_decoder_destroy(kr_audio_dec *dec);
kr_video_dec *kr_avc_video_decoder_create();
int kr_avc_video_decoder_destroy(kr_video_dec *dec);

#endif
