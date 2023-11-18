#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <krad/app/debug.h>
#include <bitstream/mpeg/ts.h>
#include <bitstream/mpeg/pes.h>
#include <bitstream/mpeg/psi/psi.h>
#include <bitstream/mpeg/psi/pmt.h>
#include <bitstream/mpeg/psi/pat.h>
#include <bitstream/mpeg/aac.h>
#include "types.h"
#include "pack.h"

struct kr_mts_muxer {
  kr_mts_info info;
  uint8_t pat[TS_SIZE];
  uint8_t pmt[TS_SIZE];
  uint8_t stuffing[TS_SIZE - TS_HEADER_SIZE_AF];
  uint8_t pkt_hdrs[KR_MTS_MUXER_VECLEN][TS_SIZE];
  uint64_t pkts;
  uint64_t pcr;
  uint8_t cc[MTS_NSTREAMS + 2];
};

static void gen_pat(kr_mts_muxer *m) {
  uint8_t *ts;
  uint8_t *pat;
  uint8_t *pat_n;
  ts = m->pat;
  memset(ts, 0x00, TS_SIZE);
  ts_init(ts);
  ts_set_unitstart(ts);
  ts_set_pid(ts, PAT_PID);
  if (m->cc[0] == 16) m->cc[0] = 0;
  ts_set_cc(ts, m->cc[0]++);
  ts_set_payload(ts);
  pat = ts + TS_HEADER_SIZE + 1;
  pat_init(pat);
  pat_set_tsid(pat, 1);
  psi_set_version(pat, 0);
  psi_set_current(pat);
  psi_set_length(pat, PSI_MAX_SIZE);
  pat_n = pat_get_program(pat, 0);
  patn_init(pat_n);
  patn_set_program(pat_n, 1);
  patn_set_pid(pat_n, m->info.programs[0].id);
  pat_n = pat_get_program(pat, 1);
  pat_set_length(pat, pat_n - pat - PAT_HEADER_SIZE);
  psi_set_section(pat, 0);
  psi_set_lastsection(pat, 0);
  psi_set_crc(pat);
  memset(ts + psi_get_length(pat) + 8,
   0xff, TS_SIZE - psi_get_length(pat) - 8);
}

 static void gen_pmt(kr_mts_muxer *m) {
  int i;
  uint8_t *ts;
  uint8_t *pmt;
  uint8_t *pmt_n;
  pmt_n = NULL;
  ts = m->pmt;
  memset(ts, 0x00, TS_SIZE);
  ts_init(ts);
  ts_set_unitstart(ts);
  ts_set_pid(ts, m->info.programs[0].id);
  if (m->cc[1] == 16) m->cc[1] = 0;
  ts_set_cc(ts, m->cc[1]++);
  ts_set_payload(ts);
  pmt = ts + TS_HEADER_SIZE + 1;
  pmt_init(pmt);
  psi_set_version(pmt, 0);
  psi_set_current(pmt);
  pmt_set_program(pmt, 1);
  pmt_set_pcrpid(pmt, m->info.programs[0].streams[0].id);
  pmt_set_desclength(pmt, 0);
  pmt_set_length(pmt, PSI_MAX_SIZE);
  for (i = 0; i < m->info.programs[0].nstreams; i++) {
    pmt_n = pmt_get_es(pmt, i);
    pmtn_init(pmt_n);
    if (m->info.programs[0].streams[i].type == MTS_VIDEO) {
      pmtn_set_streamtype(pmt_n, PMT_STREAMTYPE_VIDEO_AVC);
    } else if (m->info.programs[0].streams[i].type == MTS_AUDIO) {
      if (m->info.programs[0].streams[i].codec.audio == KR_AUDIO_CODEC_AAC) {
        pmtn_set_streamtype(pmt_n, PMT_STREAMTYPE_AUDIO_ADTS);
      } else {
        pmtn_set_streamtype(pmt_n, PMT_STREAMTYPE_AUDIO_MPEG1);
      }
    }
    pmtn_set_pid(pmt_n, m->info.programs[0].streams[i].id);
    pmtn_set_desclength(pmt_n, 0);
  }
  pmt_n = pmt_get_es(pmt, i);
  pmt_set_length(pmt, pmt_n - pmt_get_es(pmt, 0) + pmt_get_desclength(pmt));
  psi_set_crc(pmt);
  memset(ts + psi_get_length(pmt) + 8,
   0xff, TS_SIZE - psi_get_length(pmt) - 8);
 }

size_t kr_mts_muxer_sizeof() {
  return sizeof(kr_mts_muxer);
}

int kr_mts_mux_ok(kr_codec *codecs, int ntracks) {
  int i;
  if (!codecs || !ntracks) return 0;
  for (i = 0; i < ntracks; i++) {
    if (codecs[i].type == KR_CODEC_AUDIO) {
      if (codecs[i].audio == KR_AUDIO_CODEC_AAC) continue;
      if (codecs[i].audio == KR_AUDIO_CODEC_MP3) continue;
    }
    if (codecs[i].type == KR_CODEC_VIDEO) {
      if (codecs[i].video == KR_VIDEO_CODEC_H264) continue;
    }
    return 0;
  }
  return 1;
}

ssize_t kr_mts_muxer_pack_header(kr_mts_muxer *m, uint8_t *buf, size_t sz) {
  if (!m || !buf || !sz) return -1;
  if (sz < (TS_SIZE * 2)) return -2;
  memcpy(buf, m->pat, TS_SIZE);
  memcpy(buf + TS_SIZE, m->pmt, TS_SIZE);
  return TS_SIZE * 2;
}

int kr_mts_mux(kr_mts_muxer *m, kr_mts_muxer_data *data) {
  int i;
  size_t pos;
  size_t sz;
  size_t stuffing;
  uint8_t *ts;
  uint8_t *pes;
  uint64_t pcr;
  uint64_t pts;
  uint srate;
  uint samples;
  if (!m || !data) return -1;
  if (data->track >= m->info.programs[0].nstreams) return -2;
  if (!data->sz) return 0;
  memset(m->pkt_hdrs, 0, sizeof(m->pkt_hdrs));
  ts = m->pkt_hdrs[0];
  ts_init(ts);
  ts_set_unitstart(ts);
  ts_set_pid(ts, m->info.programs[0].streams[data->track].id);
  if (m->cc[2 + data->track] == 16) m->cc[2 + data->track] = 0;
  ts_set_cc(ts, m->cc[2 + data->track]++);
  ts_set_payload(ts);
  ts_set_adaptation(ts, 7);
  samples = m->info.programs[0].streams[data->track].audio.samples;
  srate = m->info.programs[0].streams[data->track].audio.sample_rate;
  pcr = 63000 + (uint64_t)((double)(90000 * samples * m->pkts++)/(double)srate);
  tsaf_set_pcr(ts, pcr);
  tsaf_set_randomaccess(ts);
  pes = ts + TS_HEADER_SIZE_PCR;
  pes_init(pes);
  pes_set_headerlength(pes, PES_HEADER_SIZE_PTS - PES_HEADER_SIZE_NOPTS);
  pes_set_length(pes, data->sz + 8);
  pes_set_streamid(pes, PES_STREAM_ID_AUDIO_MPEG);
  pts = pcr + 63000;
  pes_set_pts(pes, pts);
  data->pkts[0].hdr = m->pkt_hdrs[0];
  data->pkts[0].hdr_sz = TS_HEADER_SIZE_PCR + PES_HEADER_SIZE_PTS;
  data->pkts[0].buf = data->input;
  printkd("Data sz %zu", data->sz);
  if (data->sz < (TS_SIZE - data->pkts[0].hdr_sz)) {
    stuffing = TS_SIZE - data->pkts[0].hdr_sz - data->sz;
    printkd("Small pkt need to stuff %d", stuffing);
    pes_set_length(pes, data->sz + 8 + stuffing);
    pes_set_headerlength(pes,
     PES_HEADER_SIZE_PTS - PES_HEADER_SIZE_NOPTS + stuffing);
    memset(m->pkt_hdrs[0] + data->pkts[0].hdr_sz, 0xff,
     sizeof(m->pkt_hdrs[0]) - data->pkts[0].hdr_sz);
    data->pkts[0].hdr_sz += stuffing;
    data->pkts[0].buf = data->input;
    data->pkts[0].sz = data->sz;
    printkd("Packed %d! Header %d", data->pkts[0].sz, data->pkts[0].hdr_sz);
    ++data->npkts;
    return 0;
  }
  data->pkts[0].sz = TS_SIZE - data->pkts[0].hdr_sz;
  data->npkts++;
  pos = data->pkts[0].sz;
  sz = data->sz - data->pkts[0].sz;
  stuffing = 0;
  for (i = 1;;i++) {
    if (i == KR_MTS_MUXER_VECLEN) return -4;
    if (sz < (TS_SIZE - TS_HEADER_SIZE)) {
      if (sz <= (TS_SIZE - TS_HEADER_SIZE_AF)) {
        // pkt needs stuffing
        stuffing = TS_SIZE - TS_HEADER_SIZE_AF - sz;
        printkd("Going to stuff %d bytes", stuffing);
        data->pkts[i].hdr = m->pkt_hdrs[i];
        data->pkts[i].hdr_sz = TS_HEADER_SIZE_AF + stuffing;
        ts_init(data->pkts[i].hdr);
        ts_set_pid(data->pkts[i].hdr,
         m->info.programs[0].streams[data->track].id);
        if (m->cc[2 + data->track] == 16) m->cc[2 + data->track] = 0;
        ts_set_cc(data->pkts[i].hdr, m->cc[2 + data->track]++);
        ts_set_adaptation(data->pkts[i].hdr, stuffing + 1);
        ts_set_payload(data->pkts[i].hdr);
        data->pkts[i].buf = data->input + pos;
        data->pkts[i].sz = TS_SIZE - TS_HEADER_SIZE_AF - stuffing;
        ++data->npkts;
        break;
      } else {
        printkd("need to pack %d", sz);
        stuffing = 0;
        printkd("Going to stuff %d bytes", stuffing);
        data->pkts[i].hdr = m->pkt_hdrs[i];
        data->pkts[i].hdr_sz = TS_HEADER_SIZE_AF + stuffing;
        ts_init(data->pkts[i].hdr);
        ts_set_pid(data->pkts[i].hdr,
         m->info.programs[0].streams[data->track].id);
        if (m->cc[2 + data->track] == 16) m->cc[2 + data->track] = 0;
        ts_set_cc(data->pkts[i].hdr, m->cc[2 + data->track]++);
        ts_set_adaptation(data->pkts[i].hdr, stuffing + 1);
        ts_set_payload(data->pkts[i].hdr);
        data->pkts[i].buf = data->input + pos;
        data->pkts[i].sz = TS_SIZE - TS_HEADER_SIZE_AF - stuffing;
        printkd("packed %zu", data->pkts[i].sz);
        pos += data->pkts[i].sz;
        ++data->npkts;
        ++i;
        stuffing = 181;
        printkd("Going to stuff %d bytes", stuffing);
        data->pkts[i].hdr = m->pkt_hdrs[i];
        data->pkts[i].hdr_sz = TS_HEADER_SIZE_AF + stuffing;
        ts_init(data->pkts[i].hdr);
        ts_set_pid(data->pkts[i].hdr,
         m->info.programs[0].streams[data->track].id);
        if (m->cc[2 + data->track] == 16) m->cc[2 + data->track] = 0;
        ts_set_cc(data->pkts[i].hdr, m->cc[2 + data->track]++);
        ts_set_adaptation(data->pkts[i].hdr, stuffing + 1);
        ts_set_payload(data->pkts[i].hdr);
        data->pkts[i].buf = data->input + pos;
        data->pkts[i].sz = TS_SIZE - TS_HEADER_SIZE_AF - stuffing;
        printkd("packed %zu", data->pkts[i].sz);
        ++data->npkts;
        break;
      }
    }
    data->pkts[i].hdr = m->pkt_hdrs[i];
    data->pkts[i].hdr_sz = TS_HEADER_SIZE;
    ts_init(data->pkts[i].hdr);
    ts_set_pid(data->pkts[i].hdr,
     m->info.programs[0].streams[data->track].id);
    if (m->cc[2 + data->track] == 16) m->cc[2 + data->track] = 0;
    ts_set_cc(data->pkts[i].hdr, m->cc[2 + data->track]++);
    ts_set_payload(data->pkts[i].hdr);
    data->pkts[i].buf = data->input + pos;
    data->pkts[i].sz = TS_SIZE - TS_HEADER_SIZE;
    pos += data->pkts[i].sz;
    sz -= data->pkts[i].sz;
    printkd("Packed %d!", data->pkts[i].sz);
    ++data->npkts;
  }
  printkd("Muxed %d TS pkts", i);
  return 0;
}

kr_mts_muxer *kr_mts_muxer_init(kr_mts_muxer_setup *setup) {
  int i;
  kr_mts_muxer *mux;
  if (!setup || !setup->mem) return NULL;
  mux = (kr_mts_muxer *)setup->mem;
  memset(mux, 0, sizeof(*mux));
  if (setup->tracks == 0) return NULL;
  if (setup->tracks > KR_MTS_MUXER_NSTREAMS) {
    printk("mts_muxer: KR_MTS_MUXER_NSTREAMS exceeded!");
    return NULL;
  }
  mux->info.nprograms = 1;
  mux->info.programs[0].nstreams = setup->tracks;
  mux->info.programs[0].id = 4096;
  for (i = 0; i < setup->tracks; i++) {
    mux->info.programs[0].streams[i].id = 256 + i;
    mux->info.programs[0].streams[i].codec = setup->track_info[i].codec;
    if (setup->track_info[i].type == KR_TRACK_CODED_AUDIO) {
      if (setup->track_info[i].codec.audio != KR_AUDIO_CODEC_AAC &&
        setup->track_info[i].codec.audio != KR_AUDIO_CODEC_MP3) {
        printk("mts_muxer: wrong audio codec.");
        return NULL;
      }
      if (!setup->track_info[i].audio.sample_rate ||
       !setup->track_info[i].audio.samples) {
        printk("mts_muxer: wrong audio sample rate/samples count.");
        return NULL;
      }
      mux->info.programs[0].streams[i].audio = setup->track_info[i].audio;
      mux->info.programs[0].streams[i].type = MTS_AUDIO;
    } else if (setup->track_info[i].type == KR_TRACK_CODED_VIDEO) {
      printk("mts_muxer: video is currently not supported.");
      return NULL; /* disabling video for the time being */
      if (setup->track_info[i].codec.video != KR_VIDEO_CODEC_H264) {
        printkd("mts_muxer: wrong video codec.");
        return NULL;
      }
      mux->info.programs[0].streams[i].type = MTS_VIDEO;
    } else {
      printk("mts_muxer: wrong track type.");
      return NULL;
    }
  }
  gen_pat(mux);
  gen_pmt(mux);
  memset(mux->stuffing, 0xff, sizeof(mux->stuffing));
  return mux;
}
