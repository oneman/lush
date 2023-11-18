#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <bitstream/mpeg/ts.h>
#include <bitstream/mpeg/pes.h>
#include <bitstream/mpeg/psi/psi.h>
#include <bitstream/mpeg/psi/pmt.h>
#include <bitstream/mpeg/psi/pat.h>
#include "unpack.h"

typedef struct {
  int16_t id;
  int16_t type;
  kr_mts_stream_info *info;
} kr_mts_stream;

typedef struct {
  int16_t id;
  int16_t pmt_id;
  kr_mts_stream streams[MTS_NSTREAMS];
  int16_t nstreams;
  kr_mts_program_info *info;
} kr_mts_program;

struct kr_mts {
  uint8_t *data;
  uint8_t *curr;
  uint16_t pat_id;
  kr_mts_program programs[MTS_NPROGRAMS];
  uint8_t nprograms;
  size_t sz;
  kr_mts_info info;
  uint8_t *output;
  size_t output_pos;
  size_t output_sz;
  kr_mts_packet_info *pkt;
};

static int mts_pat_handler(kr_mts *mts) {
  uint8_t *pat;
  uint8_t *program;
  int p;
  if (!ts_get_unitstart(mts->curr)) return -1;
  if (!ts_has_payload(mts->curr)) return -1;
  pat = ts_payload(mts->curr) + 1;
  if (!pat_validate(pat)) return -1;
  printkd("Found PAT");
  p = 0;
  mts->nprograms = 0;
  mts->info.nprograms = 0;
  while ( (program = pat_get_program(pat, p)) ) {
    mts->programs[mts->nprograms].id = patn_get_program(program);
    //printk("Found program %d", mts->programs[mts->nprograms].id);
    mts->programs[mts->nprograms++].pmt_id = patn_get_pid(program);
    ++p;
    mts->info.programs[mts->info.nprograms].id = patn_get_program(program);
    mts->info.nprograms++;
    if (mts->nprograms == MTS_NPROGRAMS) {
      printke("MTS_NPROGRAMS reached!");
      return -1;
    }
  }
  return 0;
}

static kr_mts_program *mts_get_program(kr_mts *mts, int16_t pid) {
  int i;
  for (i = 0; i < mts->nprograms; i++) {
    if (pid == mts->programs[i].pmt_id) {
      mts->programs[i].info = &mts->info.programs[i];
      return &mts->programs[i];
    }
  }
  return NULL;
}

static kr_mts_stream_type mts_stream_info_type(int16_t type) {
  if (type == 0x1B) return MTS_VIDEO; /* H264 */
  if (type == 0x0F) return MTS_AUDIO; /* AAC */
  return MTS_OTHER;
}

static int mts_pmt_handler(kr_mts *mts, kr_mts_program *program) {
  kr_mts_stream *stream;
  uint8_t *pmt;
  uint8_t *pes;
  int e;
  if (!ts_get_unitstart(mts->curr)) return -1;
  if (!ts_has_payload(mts->curr)) return -1;
  pmt = ts_payload(mts->curr) + 1;
  if (!pmt_validate(pmt)) return -1;
  printkd("Found PMT");
  e = 0;
  program->nstreams = 0;
  program->info->nstreams = 0;
  while ( (pes = pmt_get_es(pmt, e++)) ) {
    stream = &program->streams[program->nstreams++];
    stream->id = pmtn_get_pid(pes);
    stream->type = pmtn_get_streamtype(pes);
    stream->info = &program->info->streams[program->info->nstreams++];
    stream->info->id = stream->id;
    stream->info->type = mts_stream_info_type(stream->type);
    strncpy(stream->info->type_str, pmt_get_streamtype_txt(stream->type),
     sizeof(stream->info->type_str));
    if (program->nstreams == MTS_NSTREAMS) {
      printke("MTS_NSTREAMS reached!");
      return -1;
    }
  }
  return 0;
}

static kr_mts_stream *mts_get_stream(kr_mts *mts, int16_t pid) {
  int i;
  int j;
  for (i = 0; i < mts->nprograms; i++) {
    for (j = 0; j < mts->programs[i].nstreams; j++) {
      if (pid == mts->programs[i].streams[j].id) {
        mts->programs[i].streams[j].info = &mts->info.programs[i].streams[j];
        mts->pkt->track = i;
        return &mts->programs[i].streams[j];
      }
    }
  }
  return NULL;
}

static int mts_pes_handler(kr_mts *mts, kr_mts_stream *stream) {
  uint8_t *pes;
  uint8_t *payload;
  size_t sz;
  if (!ts_has_payload(mts->curr)) return -1;
  if (ts_get_unitstart(mts->curr)) {
    pes = ts_payload(mts->curr);
    if (mts->output_pos > 0) return -3;
    if (!pes_validate(pes) || !pes_validate_header(pes)) return -1;
    printkd("Found PES");
    mts->pkt->type = stream->info->type;
    if (pes_has_pts(pes)) {
      mts->pkt->pts = pes_get_pts(pes);
    }
    if (pes_has_dts(pes)) {
      mts->pkt->dts = pes_get_dts(pes);
    }
    if (mts->output) {
      payload = pes_payload(pes);
      sz = TS_SIZE - (payload - mts->curr);
      if (mts->output_sz < (mts->output_pos + sz)) {
        printke("mts: Not enough output buffer.");
        return -2;
      }
      //printk("STREAM:\n%.*s", sz, payload);
      memcpy(mts->output + mts->output_pos, payload, sz);
      mts->pkt->sz += sz;
      mts->pkt->id = stream->id;
      mts->output_pos += sz;
      mts->pkt->pos = mts->curr - mts->data;
      return sz;
    }
  } else {
    if (mts->output) {
      payload = ts_payload(mts->curr);
      sz = TS_SIZE - (payload - mts->curr);
      if (mts->output_sz < (mts->output_pos + sz)) {
        printke("mts: Not enough output buffer.");
        return -2;
      }
      memcpy(mts->output + mts->output_pos, payload, sz);
      mts->pkt->sz += sz;
      mts->output_pos += sz;
      return sz;
    }
  }
  return 0;
}

size_t kr_mts_sizeof() {
  return sizeof(kr_mts);
}

int kr_mts_output_set(kr_mts *mts, uint8_t *buf, size_t sz) {
  if (!mts) return -1;
  if (!buf) return -1;
  if (sz <= 0) return -1;
  mts->output = buf;
  mts->output_sz = sz;
  mts->output_pos = 0;
  return 0;
}

int kr_mts_resync(kr_mts *mts) {
  if (!mts) return -1;
  mts->sz = 0;
  mts->data = NULL;
  mts->curr = NULL;
  return 0;
}

ssize_t kr_mts_parse(kr_mts *mts, kr_mts_packet_info *pkt,
 uint8_t *buf, size_t sz) {
  kr_mts_program *program;
  kr_mts_stream *stream;
  ssize_t ret;
  ssize_t res;
  int16_t pid;
  if (!mts || !buf) return -1;
  if (sz < TS_SIZE) return 0;
  mts->sz = sz;
  mts->data = buf;
  if (mts->curr == NULL) mts->curr = mts->data;
  mts->pkt = pkt;
  memset(mts->pkt, 0, sizeof(*mts->pkt));
  ret = 0;
  for (; mts->curr < mts->data + mts->sz; mts->curr += TS_SIZE) {
    if ( ( (mts->data + mts->sz) - mts->curr) < TS_SIZE ) break;
    if (!ts_validate(mts->curr)) {
      printke("MPEG-TS: Invalid data found!");
      ret = -1;
      break;
    }
    pid = ts_get_pid(mts->curr);
    if (pid == mts->pat_id) {
      if (mts_pat_handler(mts) != 0) {
        ret = -1;
        break;
      }
    } else if ((program = mts_get_program(mts, pid))) {
      if (mts_pmt_handler(mts, program) != 0) {
        ret = -1;
        break;
      }
    } else if ((stream = mts_get_stream(mts, pid))) {
      res = mts_pes_handler(mts, stream);
      if (res == -3) {
        break;
      } else if (res < 0) {
        ret = res;
        break;
      }
    }
    mts->info.npackets++;
    ret += TS_SIZE;
  }
  mts->output = NULL;
  mts->output_sz = 0;
  mts->output_pos = 0;
  return ret;
}

int kr_mts_info_get(kr_mts *mts, kr_mts_info *info) {
  if (!mts || !info) return -1;
  *info = mts->info;
  return 0;
}

kr_mts *kr_mts_init(kr_mts_setup *setup) {
  kr_mts *mts;
  if (!setup) return NULL;
  if (!setup->mem) return NULL;
  mts = (kr_mts *)setup->mem;
  memset(mts, 0, sizeof(*mts));
  return mts;
}
