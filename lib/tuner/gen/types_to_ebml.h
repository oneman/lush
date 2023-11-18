#if !defined(_TUNER_GEN_TYPES_TO_EBML_H)
#define _TUNER_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../hls_types.h"
#include <krad/ebml/ebml.h>
int kr_hls_variant_pack_ebml(kr_ebml *ebml, void *info);
int kr_hls_rendition_pack_ebml(kr_ebml *ebml, void *info);
int kr_hls_master_playlist_pack_ebml(kr_ebml *ebml, void *info);
int kr_hls_media_segment_pack_ebml(kr_ebml *ebml, void *info);
int kr_hls_media_playlist_pack_ebml(kr_ebml *ebml, void *info);
int kr_hls_playlist_pack_ebml(kr_ebml *ebml, void *info);
int kr_hls_tuner_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

