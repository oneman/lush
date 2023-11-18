#if !defined(_container_wave_common_H)
# define _container_wave_common_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <krad/container/packet.h>
#include <krad/codec/types.h>

#define WAVE_HEADER_MIN_SIZE 44

typedef enum {
  KR_WAVE_PCM = 0x0001,
  KR_WAVE_IEEE_FLOAT = 0x0003,
  KR_WAVE_ALAW = 0x0006,
  KR_WAVE_MULAW = 0x0007,
  KR_WAVE_EXTENSIBLE = 0xFFFE
} kr_wave_fmt;

typedef struct {
  kr_wave_fmt fmt;
  int extensible;
  kr_audio_info audio;
  size_t data_sz;
  size_t hdr_sz;
} kr_wave_info;

#endif
