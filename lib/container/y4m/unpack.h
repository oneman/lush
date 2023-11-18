#if !defined(_container_y4m_unpack_H)
# define _container_y4m_unpack_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <krad/container/packet.h>
#include <krad/codec/types.h>
#include "common.h"

typedef struct kr_y4m_demuxer kr_y4m_demuxer;

typedef struct {
  uint8_t *header;
  size_t header_sz;
  uint8_t *mem;
} kr_y4m_demuxer_setup;

size_t kr_y4m_demuxer_sizeof();
ssize_t kr_y4m_demuxer_read_header(kr_y4m_demuxer *y, uint8_t *header,
 size_t sz);
ssize_t kr_y4m_demuxer_read_frame_header(kr_y4m_demuxer *y, uint8_t *buf,
 size_t sz);
kr_y4m_demuxer *kr_y4m_demuxer_init(kr_y4m_demuxer_setup *setup);
int kr_y4m_demuxer_get_image_info(kr_y4m_demuxer *d, kr_image_info *info);
int kr_y4m_demuxer_get_fps(kr_y4m_demuxer *d, kr_fps_info *fps);
/*size_t kr_y4m_demuxer_unpack_frame(kr_y4m_demuxer *d, kr_frame *frame, uint8_t *buf, size_t sz);*/

#endif
