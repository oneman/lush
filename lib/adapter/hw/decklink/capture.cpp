#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#include "DeckLinkAPI.h"
#include "capture.h"

struct krad_decklink_capture_St {
  IDeckLink *deckLink;
  IDeckLinkInput *deckLinkInput;
  IDeckLinkConfiguration *deckLinkConfiguration;
  IDeckLinkIterator *deckLinkIterator;
  IDeckLinkDisplayModeIterator *displayModeIterator;
  IDeckLinkDisplayMode *displayMode;
  DeckLinkCaptureDelegate *delegate;
  HRESULT result;
  BMDDisplayMode display_mode;
  BMDPixelFormat pixel_format;
  BMDAudioSampleRate audio_sample_rate;
  BMDAudioConnection audio_input;
  BMDVideoConnection video_input;
  int inputFlags;
  int verbose;
  int skip_frame;
  int skipped_frames;
  int audio_channels;
  int audio_bit_depth;
  uint64_t video_frames;
  uint64_t audio_frames;
  int (*video_frame_callback)(void *, kr_image *);
  int (*audio_frames_callback)(void *, void *, int);
  void *callback_pointer;
  int width;
  int height;
  int fps_numerator;
  int fps_denominator;
  int device;
};

DeckLinkCaptureDelegate::DeckLinkCaptureDelegate() : refs(0) {}
DeckLinkCaptureDelegate::~DeckLinkCaptureDelegate() {}

#define KR_DL_MM __ATOMIC_SEQ_CST

ULONG DeckLinkCaptureDelegate::AddRef(void) {
  int r;
  r = __atomic_add_fetch(&refs, 1, KR_DL_MM);
  printk("Decklink: capture refs now %zu", r);
  return r;
}

ULONG DeckLinkCaptureDelegate::Release(void) {
  int r;
  r = __atomic_sub_fetch(&refs, 1, KR_DL_MM);
  printk("Decklink: capture refs now %zu", r);
  if (r == 0) delete this;
  return r;
}

HRESULT DeckLinkCaptureDelegate::VideoInputFrameArrived(
 IDeckLinkVideoInputFrame *video_frame, IDeckLinkAudioInputPacket *audio_frame) {
  kr_image image;
  void *audio_data;
  IDeckLinkTimecode *timecode;
  BMDTimecodeFormat timecodeFormat;
  int audio_frames;
  const char *timecodeString;
  timecodeString = NULL;
  timecodeFormat = 0;
  if ((audio_frame == NULL) || (video_frame == NULL) || (video_frame->GetFlags() & bmdFrameHasNoInputSource)) {
    if (krad_decklink_capture->skipped_frames == 0) {
      printke("Decklink A/V Capture skipped a frame!");
    }
    krad_decklink_capture->skipped_frames++;
  }
  if (krad_decklink_capture->skipped_frames > 0) {
    printke("Decklink A/V Capture got a frame after skipping %d", krad_decklink_capture->skipped_frames);
    krad_decklink_capture->skipped_frames = 0;
  }
  if (audio_frame) {
    audio_frame->GetBytes(&audio_data);
    audio_frames = audio_frame->GetSampleFrameCount();
    if (krad_decklink_capture->audio_frames_callback != NULL) {
      krad_decklink_capture->audio_frames_callback(krad_decklink_capture->callback_pointer, audio_data, audio_frames);
    }
    krad_decklink_capture->audio_frames += audio_frames;
  }
  if (krad_decklink_capture->skip_frame == 1) {
    krad_decklink_capture->skip_frame = 0;
    return S_OK;
  }
  if (krad_decklink_capture->skip_frame == 0) {
    if ((krad_decklink_capture->display_mode == bmdModeHD720p60) ||
      (krad_decklink_capture->display_mode == bmdModeHD720p5994) ||
      (krad_decklink_capture->display_mode == bmdModeHD1080p5994) ||
      (krad_decklink_capture->display_mode == bmdModeHD1080p6000)) {
      krad_decklink_capture->skip_frame = 1;
    }
  }
  if (video_frame) {
    if (timecodeFormat != 0) {
      if (video_frame->GetTimecode(timecodeFormat, &timecode) == S_OK) {
        timecode->GetString(&timecodeString);
      }
    }
    //frame_data_size = video_frame->GetRowBytes() * video_frame->GetHeight();
    if (timecodeString) {
      free((void*)timecodeString);
    }
    if (krad_decklink_capture->video_frame_callback != NULL) {
      memset(&image, 0, sizeof(kr_image));
      video_frame->GetBytes((void **)&image.px[0]);
      image.ps[0] = video_frame->GetRowBytes();
      image.info.w = krad_decklink_capture->width;
      image.info.h = krad_decklink_capture->height;
      image.info.fmt = KR_PIXELS_YVU422P;
      krad_decklink_capture->video_frame_callback(krad_decklink_capture->callback_pointer, &image);
    }
    krad_decklink_capture->video_frames++;
  }
  return S_OK;
}

HRESULT DeckLinkCaptureDelegate::VideoInputFormatChanged(
 BMDVideoInputFormatChangedEvents events, IDeckLinkDisplayMode *mode,
 BMDDetectedVideoInputFormatFlags) {
  printke("Decklink: ruh oh! video format changed?!?\n");
  return S_OK;
}

extern "C" {

krad_decklink_capture_t *krad_decklink_capture_create(int device) {
  int d;
  krad_decklink_capture_t *krad_decklink_capture;
  krad_decklink_capture = (krad_decklink_capture_t *)kr_allocz(1, sizeof(krad_decklink_capture_t));
  krad_decklink_capture->device = device;
  krad_decklink_capture->inputFlags = 0;
  krad_decklink_capture->audio_sample_rate = bmdAudioSampleRate48kHz;
  krad_decklink_capture->audio_channels = 2;
  krad_decklink_capture->audio_bit_depth = 16;
  krad_decklink_capture->deckLinkIterator = CreateDeckLinkIteratorInstance();
  if (!krad_decklink_capture->deckLinkIterator) {
    printke("Decklink: No DeckLink drivers installed.");
    free(krad_decklink_capture);
    return NULL;
  }
  for (d = 0; d < 64; d++) {
    krad_decklink_capture->result = krad_decklink_capture->deckLinkIterator->Next(&krad_decklink_capture->deckLink);
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: No DeckLink devices found.");
      free(krad_decklink_capture);
      return NULL;
    }
    if (d == krad_decklink_capture->device) {
      break;
    }
  }
  return krad_decklink_capture;
}

void set_audio_input(kr_decklink_capture *cap,
 kr_decklink_audio_conn_type conn) {
  switch (conn) {
    case KR_DL_EMBEDDED:
      cap->audio_input = bmdAudioConnectionEmbedded;
      printk("Decklink: Selected embedded audio input");
      break;
    case KR_DL_ANALOG:
      cap->audio_input = bmdAudioConnectionAnalog;
      printk("Decklink: Selected analog audio input");
      break;
    case KR_DL_AESEBU:
      cap->audio_input = bmdAudioConnectionAESEBU;
      printk("Decklink: Selected AESEBU audio input");
      break;
    default:
      cap->audio_input = bmdAudioConnectionEmbedded;
      printke("Decklink: Unknown audio input defaulting to embedded input");
      break;
  }
} 

void set_video_input(kr_decklink_capture *cap,
 kr_decklink_video_conn_type conn) {
  switch (conn) {
    case KR_DL_SDI:
      cap->video_input = bmdVideoConnectionSDI;
      printk("Decklink: Selected SDI video input");
      break;
    case KR_DL_OPTICAL_SDI:
      cap->video_input = bmdVideoConnectionOpticalSDI;
      printk("Decklink: Selected Optical SDI video input");
      break;
    case KR_DL_HDMI:
      cap->video_input = bmdVideoConnectionHDMI;
      printk("Decklink: Selected HDMI video input");
      break;
    case KR_DL_COMPONENT:
      cap->video_input = bmdVideoConnectionComponent;
      printk("Decklink: Selected component video input");
      break;
    case KR_DL_COMPOSITE:
      cap->video_input = bmdVideoConnectionComposite;  
      printk("Decklink: Selected composite video input");
      break;
    case KR_DL_SVIDEO:
      cap->video_input = bmdVideoConnectionSVideo;
      printk("Decklink: Selected svideo video input");
      break;
    default:
      cap->video_input = bmdVideoConnectionSDI;
      printke("Decklink: Unknown video input defaulting to SDI");
      break;
  }
}

void set_video_mode(krad_decklink_capture_t *krad_decklink_capture,
 int width, int height, int fps_numerator, int fps_denominator) {
  krad_decklink_capture->width = width;
  krad_decklink_capture->height = height;
  krad_decklink_capture->fps_numerator = fps_numerator;
  krad_decklink_capture->fps_denominator = fps_denominator;
  krad_decklink_capture->pixel_format = bmdFormat8BitYUV;
  //krad_decklink_capture->pixel_format = bmdFormat8BitARGB;
  if ((krad_decklink_capture->width == 720) && (krad_decklink_capture->height == 486)) {
    if ((krad_decklink_capture->fps_numerator == 30000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdModeNTSC;
    }
    if ((krad_decklink_capture->fps_numerator == 60000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdModeNTSCp;
    }
  }
  if ((krad_decklink_capture->width == 720) && (krad_decklink_capture->height == 576)) {
    if (((krad_decklink_capture->fps_numerator == 25) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 25000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModePAL;
    }
    if (((krad_decklink_capture->fps_numerator == 50) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 50000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModePALp;
    }
  }
  if ((krad_decklink_capture->width == 1280) && (krad_decklink_capture->height == 720)) {
    if (((krad_decklink_capture->fps_numerator == 50) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 50000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD720p50;
    }
    if ((krad_decklink_capture->fps_numerator == 60000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdModeHD720p5994;
    }
    if (((krad_decklink_capture->fps_numerator == 60) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 60000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD720p60;
    }
  }
  if ((krad_decklink_capture->width == 1920) && (krad_decklink_capture->height == 1080)) {
    if ((krad_decklink_capture->fps_numerator == 24000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdModeHD1080p2398;
    }
    if (((krad_decklink_capture->fps_numerator == 24) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 24000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD1080p24;
    }
    if (((krad_decklink_capture->fps_numerator == 25) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 25000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD1080p25;
    }
    if ((krad_decklink_capture->fps_numerator == 30000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdModeHD1080p2997;
    }
    if (((krad_decklink_capture->fps_numerator == 30) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 30000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD1080p30;
    }
    if (((krad_decklink_capture->fps_numerator == 50) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 50000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD1080p50;
    }
    if ((krad_decklink_capture->fps_numerator == 60000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdModeHD1080p5994;
    }
    if (((krad_decklink_capture->fps_numerator == 60) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 60000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdModeHD1080p6000;
    }
  }
  if ((krad_decklink_capture->width == 2048) && (krad_decklink_capture->height == 1556)) {
    if ((krad_decklink_capture->fps_numerator == 24000) && (krad_decklink_capture->fps_denominator == 1001)) {
      krad_decklink_capture->display_mode = bmdMode2k2398;
    }
    if (((krad_decklink_capture->fps_numerator == 24) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 24000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdMode2k24;
    }
    if (((krad_decklink_capture->fps_numerator == 25) && (krad_decklink_capture->fps_denominator == 1)) ||
       ((krad_decklink_capture->fps_numerator == 25000) && (krad_decklink_capture->fps_denominator == 1000))) {
      krad_decklink_capture->display_mode = bmdMode2k25;
    }
  }
}

void capture_stop(krad_decklink_capture_t *krad_decklink_capture) {
    if (krad_decklink_capture->deckLinkInput != NULL) {
    krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->StopStreams();
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Fail StopStreams\n");
    }
    krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->DisableVideoInput();
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Fail DisableVideoInput\n");
    }
    krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->DisableAudioInput();
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Fail DisableAudioInput\n");
    }
    krad_decklink_capture->result = krad_decklink_capture->deckLinkConfiguration->Release();
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Fail to Release deckLinkConfiguration\n");
    }
    krad_decklink_capture->deckLinkConfiguration = NULL;
    krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->Release();
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Fail Release\n");
    }
        krad_decklink_capture->deckLinkInput = NULL;
    }
    if (krad_decklink_capture->deckLink != NULL) {
        krad_decklink_capture->deckLink->Release();
        krad_decklink_capture->deckLink = NULL;
    }
  if (krad_decklink_capture->deckLinkIterator != NULL) {
    krad_decklink_capture->deckLinkIterator->Release();
  }
  free(krad_decklink_capture);
}

void capture_start(krad_decklink_capture_t *krad_decklink_capture) {
  krad_decklink_capture->result = krad_decklink_capture->deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&krad_decklink_capture->deckLinkInput);
  if (krad_decklink_capture->result != S_OK) {
    printke("Decklink: Fail QueryInterface\n");
  }
  krad_decklink_capture->result = krad_decklink_capture->deckLink->QueryInterface(IID_IDeckLinkConfiguration, (void**)&krad_decklink_capture->deckLinkConfiguration);
  if (krad_decklink_capture->result != S_OK) {
    printke("Decklink: Fail QueryInterface to get configuration\n");
  } else {
    krad_decklink_capture->result = krad_decklink_capture->deckLinkConfiguration->SetInt(bmdDeckLinkConfigAudioInputConnection, krad_decklink_capture->audio_input);
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Failed to set audio input connection");
    }
    krad_decklink_capture->result = krad_decklink_capture->deckLinkConfiguration->SetInt(bmdDeckLinkConfigVideoInputConnection, krad_decklink_capture->video_input);
    if (krad_decklink_capture->result != S_OK) {
      printke("Decklink: Failed to set video input connection");
    }
  }
  krad_decklink_capture->delegate = new DeckLinkCaptureDelegate();
  krad_decklink_capture->delegate->krad_decklink_capture = krad_decklink_capture;
  krad_decklink_capture->deckLinkInput->SetCallback(krad_decklink_capture->delegate);
  krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->EnableVideoInput(krad_decklink_capture->display_mode, krad_decklink_capture->pixel_format, krad_decklink_capture->inputFlags);
  if (krad_decklink_capture->result != S_OK) {
    printke("Decklink: Fail EnableVideoInput\n");
  }
  krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->EnableAudioInput(krad_decklink_capture->audio_sample_rate, krad_decklink_capture->audio_bit_depth, krad_decklink_capture->audio_channels);
  if (krad_decklink_capture->result != S_OK) {
    printke("Decklink: Fail EnableAudioInput\n");
  }
  krad_decklink_capture->result = krad_decklink_capture->deckLinkInput->StartStreams();
  if (krad_decklink_capture->result != S_OK) {
    printke("Decklink: Fail StartStreams\n");
  }
}

int get_device_name(int num, char *name) {
  IDeckLinkIterator *iter;
  IDeckLink *dev;
  HRESULT res;
  int i;
  char *str;
  int ret;
  str = NULL;
  i = 0;
  ret = 0;
  iter = CreateDeckLinkIteratorInstance();
  if (!iter) {
    printke("Decklink: The DeckLink drivers may not be installed.");
    return -1;
  }
  while (iter->Next(&dev) == S_OK) {
    if (i == num) {
      res = dev->GetModelName((const char **) &str);
      dev->Release();
      if (res == S_OK) {
        strcpy(name, str);
        free(str);
        ret = strlen(name);
      } else {
        strcpy(name, "Unknown Error in GetModelName");
      }
      break;
    } else {
      dev->Release();
      i++;
    }
  }
  iter->Release();
  if (ret == 0) {
    sprintf(name, "Could not get name for device %d", num);
  }
  return ret;
}

int kr_decklink_capture_close(kr_decklink_capture *capture) {
  if (!capture) return -1;
  capture_stop(capture);
  return 0;
}  
  
kr_decklink_capture *kr_decklink_capture_open(kr_decklink_setup *setup) {
  kr_decklink_capture *cap;
  if (!setup) return NULL;
  cap = krad_decklink_capture_create(setup->info->num);
  if (!cap) return NULL;
  set_audio_input(cap, setup->path_info->audio_conn);
  set_video_input(cap, setup->path_info->video_conn);
  set_video_mode(cap, setup->path_info->width, setup->path_info->height,
   setup->path_info->num, setup->path_info->den);
  cap->video_frame_callback = setup->frame_cb;
  cap->audio_frames_callback = setup->audio_cb;
  cap->callback_pointer = setup->user;
  cap->verbose = 1;
  capture_start(cap);
  return cap;
}

int kr_decklink_get_name(kr_decklink_info *info) {
  int ret;
  if (!info) return -1;
  ret = get_device_name(info->num, info->name);
  return ret;
}

/* extern c */
}
