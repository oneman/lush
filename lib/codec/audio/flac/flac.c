#include <krad/mem/mem.h>
#include "flac.h"

/* Encoding */

static FLAC__StreamEncoderSeekStatus
krad_flac_encoder_seek_callback (const FLAC__StreamEncoder *encoder,
                                 FLAC__uint64 absolute_byte_offset,
                                 void *client_data) {

  krad_flac_t *flac = (krad_flac_t *)client_data;

  //printfkd ("got seek callback %lu\n", absolute_byte_offset);
  if (absolute_byte_offset < 666 + 23) {
    flac->streaminfo_rewrite = absolute_byte_offset - 666;
    return FLAC__STREAM_ENCODER_SEEK_STATUS_OK;
  } else {
       return FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR;
  }
}

static FLAC__StreamEncoderTellStatus
krad_flac_encoder_tell_callback (const FLAC__StreamEncoder *encoder,
                                 FLAC__uint64 *absolute_byte_offset,
                                 void *client_data) {

  *absolute_byte_offset = 666;
  //printk ("got tell callback\n");
  return FLAC__STREAM_ENCODER_TELL_STATUS_OK;
}

static FLAC__StreamEncoderWriteStatus
krad_flac_encoder_write_callback (const FLAC__StreamEncoder *encoder,
                                  const FLAC__byte fbuffer[],
                                  size_t bytes, unsigned samples,
                                  unsigned current_frame, void *client_data) {

  krad_flac_t *flac = (krad_flac_t *)client_data;

  //printk ("Flac write callback with %d bytes,
  //          %d samples, %d current frame \n", bytes, samples, current_frame);

  // additional header metadata
  // this code is above so we dont write streaminfo block twice..
  if ((samples == 0) && (flac->have_min_header == 1) &&
      (flac->total_frames == 0) && (flac->streaminfo_rewrite == 0)) {
    //printk ("additional!! %d\n", bytes);
    memcpy(flac->header + flac->header_size, fbuffer, bytes);
    flac->header_size += bytes;
  }

  // Initial streaminfo block
  if ((samples == 0) && (flac->have_min_header == 0) &&
      (bytes == KRAD_FLAC_STREAMINFO_BLOCK_SIZE)) {
    // flac METADATA_BLOCK_STREAMINFO
    memcpy (flac->streaminfo_block, fbuffer, KRAD_FLAC_STREAMINFO_BLOCK_SIZE);
    memcpy (flac->header, KRAD_FLAC_MARKER, 4);
    memcpy (flac->header + 4,
            flac->streaminfo_block,
            KRAD_FLAC_STREAMINFO_BLOCK_SIZE);

    memcpy (flac->min_header, flac->header, KRAD_FLAC_MINIMAL_HEADER_SIZE);

    // the following marks the streaminfo block as the final metadata block
    // for the minimal header only!
    flac->min_header[4] = '\x80';

    flac->header_size = KRAD_FLAC_MINIMAL_HEADER_SIZE;
    flac->have_min_header = 1;

    flac->krad_codec_header.codec = FLAC;
    // combined header just has fLaC + streaminfo and is
    // marked as final metadata
    //flac->krad_codec_header.header_combined = flac->min_header;
    //flac->krad_codec_header.header_combined_size =
    //KRAD_FLAC_MINIMAL_HEADER_SIZE;

    // split headers are 1 = fLaC + streaminfo not marked as final
    // and 2 = a vorbis comment
    flac->krad_codec_header.data[0] = flac->header;
    flac->krad_codec_header.sz[0] = KRAD_FLAC_MINIMAL_HEADER_SIZE;

    flac->krad_codec_header.sz[1] =
    4 + 4 + strlen (FLAC__VENDOR_STRING) + 4 + 4 +
    strlen ("ENCODER=") + strlen (KR_VERSION_STR_FULL);

    flac->comment_header = kr_allocz (1, flac->krad_codec_header.sz[1]);

    memcpy (flac->comment_header, "\x84\x00\x00", 3);

    flac->comment_header[3] = flac->krad_codec_header.sz[1] - 4;

    flac->comment_header[4] = strlen (FLAC__VENDOR_STRING);

    memcpy (flac->comment_header + 8,
            FLAC__VENDOR_STRING,
            strlen (FLAC__VENDOR_STRING));

    flac->comment_header[8 + strlen (FLAC__VENDOR_STRING)] = 1;

    flac->comment_header[8 + strlen (FLAC__VENDOR_STRING) + 4] =
    strlen ("ENCODER=") + strlen (KR_VERSION_STR_FULL);

    memcpy (flac->comment_header + 8 + strlen (FLAC__VENDOR_STRING) + 4 + 4,
            "ENCODER=",
            strlen ("ENCODER="));

    memcpy (flac->comment_header + 8 + strlen (FLAC__VENDOR_STRING) +
            4 + 4 + strlen ("ENCODER="),
            KR_VERSION_STR_FULL,
            strlen (KR_VERSION_STR_FULL));
    flac->krad_codec_header.data[1] = flac->comment_header;
    flac->krad_codec_header.count = 2;
  }

  // needs to be above the below
  if (flac->streaminfo_rewrite == 0) {
    flac->bytes += bytes;
    flac->total_bytes += bytes;
    if (flac->encode_buffer != NULL) {
      memcpy(flac->encode_buffer, fbuffer, bytes);
    }
  }

  // rewrites streaminfo block with samples, min/max blocksize and md5
  if (flac->streaminfo_rewrite) {
    // printk ("yeehaw %d bytes on ye streaminfo rewrite at %d\n",
    // bytes, flac->streaminfo_rewrite);
    /*
    if (bytes == 16) {
      int b;
      char testbuffer[256];

      // printk ("flac encoder write callback called with 0 samples,
      // this is metadata\n");
      if (bytes < 128) {
        memset(testbuffer, '\0', 256);
        for(b = 0; b < bytes; b++) {
          sprintf(testbuffer + (b * 2), "%02x", fbuffer[b]);
        }
      }
      //printk (" --%s-- \n", testbuffer);

    }

    memcpy (flac->streaminfo_block + flac->streaminfo_rewrite,
            fbuffer,
            bytes);
    memcpy (flac->min_header + 4,
            flac->streaminfo_block,
            KRAD_FLAC_STREAMINFO_BLOCK_SIZE);
    // the following marks the streaminfo block as the final metadata block
    flac->min_header[4] = '\x80';
    memcpy (flac->header + 4,
            flac->streaminfo_block,
            KRAD_FLAC_STREAMINFO_BLOCK_SIZE);
    */
    flac->streaminfo_rewrite = 0;
  }

  if (samples > 0) {
    flac->total_frames += samples;
  }
  return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

int krad_flac_encode (krad_flac_t *flac,
                      float *audio, int frames,
                      uint8_t *encode_buffer) {

  int ret;
  int num_samples;
  int s;
  int bitshifter;
  double scaled_value;

  if (flac->bit_depth == 24) {
    bitshifter = 8;
  } else {
    bitshifter = 16;
  }

  num_samples = frames * flac->channels;

  for (s = 0; s < num_samples; s++) {
    scaled_value = (audio[s] * (8.0 * 0x10000000));
    flac->int32_samples[s] = (int) (lrint (scaled_value) >> bitshifter);
  }

  flac->bytes = 0;
  flac->encode_buffer = encode_buffer;

  flac->total_frames_input += frames;

  FLAC__stream_encoder_process_interleaved (flac->encoder,
                       (const FLAC__int32 *)flac->int32_samples,
                                            frames);

  ret = flac->bytes;
  flac->bytes = 0;

  return ret;
}

int krad_flac_encoder_finish (krad_flac_t *flac, uint8_t *encode_buffer) {

  flac->bytes = 0;

  if (encode_buffer != NULL) {
    flac->encode_buffer = encode_buffer;
  } else {
    flac->encode_buffer = NULL;
  }

  if (flac->finished == 0) {
    FLAC__stream_encoder_finish (flac->encoder);
    flac->finished = 1;
  }

  return flac->bytes;
}

void krad_flac_encoder_destroy (krad_flac_t *flac) {

  if (flac->comment_header != NULL) {
    free (flac->comment_header);
  }
  krad_flac_encoder_finish (flac, NULL);
  FLAC__stream_encoder_delete (flac->encoder);
  free (flac);
}

krad_flac_t *krad_flac_encoder_create (int channels,
                                       int sample_rate,
                                       int bit_depth) {

  krad_flac_t *flac = kr_allocz (1, sizeof(krad_flac_t));

  flac->channels = channels;
  flac->sample_rate = sample_rate;
  flac->bit_depth = bit_depth;

  flac->encoder = FLAC__stream_encoder_new ();

  FLAC__stream_encoder_set_channels (flac->encoder, flac->channels);
  FLAC__stream_encoder_set_bits_per_sample (flac->encoder, flac->bit_depth);
  FLAC__stream_encoder_set_sample_rate (flac->encoder, flac->sample_rate);
  FLAC__stream_encoder_set_compression_level  (flac->encoder, 7);

  FLAC__StreamEncoderInitStatus ret =
  FLAC__stream_encoder_init_stream (flac->encoder,
                                    krad_flac_encoder_write_callback,
                                    krad_flac_encoder_seek_callback,
                                    krad_flac_encoder_tell_callback,
                                    NULL,
                                    flac);

  if (ret != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
    failfast ("Krad FLAC Encoder failure on create");
  }

  return flac;
}

int krad_flac_encoder_frames_remaining (krad_flac_t *flac) {
  return flac->total_frames_input - flac->total_frames;
}

void krad_flac_encode_info (krad_flac_t *flac) {
  printk ("Krad FLAC Encoder info:");
  printk ("Encoded Frames: %llu", flac->total_frames);
  printk ("Encoded Bytes: %llu", flac->total_bytes);
  printk ("Sample Rate: %d", flac->sample_rate);
  printk ("Bit Depth: %d", flac->bit_depth);
  printk ("Channels: %d", flac->channels);
}

int krad_flac_encoder_read_min_header (krad_flac_t *flac,
                                       uint8_t *buffer) {
  memcpy (buffer, flac->min_header, KRAD_FLAC_MINIMAL_HEADER_SIZE);
  return KRAD_FLAC_MINIMAL_HEADER_SIZE;
}

int krad_flac_encoder_read_header (krad_flac_t *flac, uint8_t *buffer) {
  memcpy (buffer, flac->header, flac->header_size);
  return flac->header_size;
}

/* Decoding */

static void krad_int24_to_float_array (const int *in,
                                       float *out,
                                       int len) {
  while (len)  {
    len--;
    out[len] = (float) ((in[len] << 8) / (8.0 * 0x10000000));
  };
}

static void krad_int16_to_float_array (const int *in,
                                       float *out,
                                       int len) {
  while (len)  {
    len--;
    out[len] = (float) ((in[len] << 16) / (8.0 * 0x10000000));
  };
}

static void krad_flac_decoder_info (krad_flac_t *flac) {
  printk ("Krad FLAC Decoder info:");
  printk ("Frames decoded: %llu", flac->total_frames_input);
  printk ("Total frames: %llu", flac->total_frames);
  printk ("Sample Rate: %d", flac->sample_rate);
  printk ("Bit Depth: %d", flac->bit_depth);
  printk ("Channels: %d", flac->channels);
}

static void krad_flac_decoder_cb_error (const FLAC__StreamDecoder *flacdecoder,
                                        FLAC__StreamDecoderErrorStatus status,
                                        void *client_data) {

  //krad_flac_t *flac = (krad_flac_t *)client_data;

  failfast ("Krad FLAC Decoder failure: %s\n",
            FLAC__StreamDecoderErrorStatusString[status]);
}

static void krad_flac_decoder_cb_metadata (const FLAC__StreamDecoder *decoder,
                                           const FLAC__StreamMetadata *metadata,
                                           void *client_data) {

  krad_flac_t *flac = (krad_flac_t *)client_data;

  flac->channels = metadata->data.stream_info.channels;
  flac->sample_rate = metadata->data.stream_info.sample_rate;
  flac->bit_depth = metadata->data.stream_info.bits_per_sample;

  if (metadata->data.stream_info.total_samples != 0) {
    flac->total_frames = metadata->data.stream_info.total_samples;
    //printk ("Krad FLAC Decoder: Total frames of %llu\n", flac->total_frames);
  }

  krad_flac_decoder_info (flac);
}

static FLAC__StreamDecoderWriteStatus
krad_flac_decoder_callback_write (const FLAC__StreamDecoder *flacdecoder,
                                  const FLAC__Frame *frame,
                                  const FLAC__int32 * const fbuffer[],
                                  void *client_data) {

  krad_flac_t *flac = (krad_flac_t *)client_data;

  int c;

  //printk ("Krad FLAC Decoder: write callback with %d frames\n",
  //         frame->header.blocksize);

  flac->frames = frame->header.blocksize;
  flac->total_frames_input += flac->frames;

  for (c = 0; c < flac->channels; c++) {
    if (flac->bit_depth == 16) {
      krad_int16_to_float_array ((const int *)fbuffer[c],
                                 flac->output_buffer[c],
                                 frame->header.blocksize);
    } else {
      krad_int24_to_float_array ((const int *)fbuffer[c],
                                 flac->output_buffer[c],
                                 frame->header.blocksize);
    }
  }
  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static FLAC__StreamDecoderReadStatus
krad_flac_decoder_callback_read (const FLAC__StreamDecoder *flacdecoder,
                                 FLAC__byte fbuffer[],
                                 size_t *bytes,
                                 void *client_data) {

  krad_flac_t *flac = (krad_flac_t *)client_data;

  //printk ("Krad FLAC Decoder: read callback read with %zu bytes\n", *bytes);

  if (!(*bytes <= (flac->decode_buffer_len - flac->decode_buffer_pos))) {
    *bytes = flac->decode_buffer_len - flac->decode_buffer_pos;
  }

  memcpy (fbuffer, flac->decode_buffer + flac->decode_buffer_pos, *bytes);
  flac->decode_buffer_pos += *bytes;

  return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}

int krad_flac_decode (krad_flac_t *flac,
                      uint8_t *encoded_buffer, int len,
                      float **audio) {

  FLAC__bool ret;

  memcpy (flac->decode_buffer + flac->decode_buffer_len,
          encoded_buffer,
          len);
  flac->decode_buffer_len += len;
  flac->output_buffer = audio;

  ret = FLAC__stream_decoder_process_single ( flac->decoder );

  if (ret == false) {
    failfast ("Krad FLAC Decoder failure on decode");
  }

  if (flac->decode_buffer_len != flac->decode_buffer_pos) {
    printke ("Krad FLAC Decoder failure on decode len %d pos %d",
             flac->decode_buffer_len, flac->decode_buffer_pos);
    memmove (flac->decode_buffer,
             flac->decode_buffer + flac->decode_buffer_pos,
             flac->decode_buffer_len - flac->decode_buffer_pos);
    flac->decode_buffer_len -= flac->decode_buffer_pos;
    flac->decode_buffer_pos = 0;
  } else {
    flac->decode_buffer_pos = 0;
    flac->decode_buffer_len = 0;
  }
  printk ("Krad FLAC Decoded %d", flac->frames);
  return flac->frames;
}

krad_flac_t *krad_flac_decoder_create (krad_codec_header_t *header) {

  int h;
  krad_flac_t *flac = kr_allocz (1, sizeof(krad_flac_t));
  FLAC__StreamDecoderInitStatus ret;

  flac->channels = 2;
  flac->bit_depth = 16;

  flac->decoder = FLAC__stream_decoder_new ();

  flac->decode_buffer = kr_alloc (8192 * 64);

  ret =
  FLAC__stream_decoder_init_stream (flac->decoder,
                                    krad_flac_decoder_callback_read,
                                    NULL, NULL, NULL, NULL,
                                    krad_flac_decoder_callback_write,
                                    krad_flac_decoder_cb_metadata,
                                    krad_flac_decoder_cb_error,
                                    flac );

  if (ret != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
    failfast ("Krad FLAC Decoder failure on create");
  }

  if (header != NULL) {
    for (h = 0; h < header->count; h++) {
      if ((header->sz[h] > 0) && (header->data[h] != NULL)) {
        krad_flac_decode (flac, header->data[h], header->sz[h], NULL);
      } else {
        break;
      }
    }
  }

  return flac;
}

void krad_flac_decoder_destroy (krad_flac_t *flac) {
  FLAC__stream_decoder_finish ( flac->decoder );
  FLAC__stream_decoder_delete ( flac->decoder );
  free (flac->decode_buffer);
  free (flac);
}

int krad_flac_decoder_test (uint8_t *header_buffer, int len) {

  krad_flac_t *flac;
  krad_codec_header_t header;

  header.data[0] = header_buffer;
  header.sz[0] = len;
  header.count = 1;

  flac = krad_flac_decoder_create (&header);

  if (flac != NULL) {
    krad_flac_decoder_destroy (flac);
    return 0;
  }
  return -1;
}
