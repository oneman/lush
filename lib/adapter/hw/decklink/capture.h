#ifndef __CAPTURE_H__
#define __CAPTURE_H__

typedef struct krad_decklink_capture_St krad_decklink_capture_t;
typedef struct krad_decklink_capture_St kr_decklink_capture;

#ifdef __cplusplus
extern "C" {
#include "types.h"
#include <krad/image/image.h>
#include <krad/image/convert.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>
}
#endif

#ifdef __cplusplus

#include "DeckLinkAPI.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

class DeckLinkCaptureDelegate : public IDeckLinkInputCallback {
  public:
    DeckLinkCaptureDelegate();
    ~DeckLinkCaptureDelegate();

  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv) {
    return E_NOINTERFACE; 
  }
  virtual ULONG STDMETHODCALLTYPE AddRef(void);
  virtual ULONG STDMETHODCALLTYPE  Release(void);
  virtual HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(
   BMDVideoInputFormatChangedEvents, IDeckLinkDisplayMode *,
   BMDDetectedVideoInputFormatFlags);
  virtual HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(
   IDeckLinkVideoInputFrame *, IDeckLinkAudioInputPacket*);

  kr_decklink_capture *krad_decklink_capture;

  private:
    ULONG refs;
};

extern "C" {
#endif

typedef struct {
  int (*frame_cb)(void *, kr_image *);
  int (*audio_cb)(void *, void *, int);
  void *user;
  kr_decklink_info *info;
  kr_decklink_path_info *path_info;
} kr_decklink_setup;

int kr_decklink_capture_close(kr_decklink_capture *capture);
kr_decklink_capture *kr_decklink_capture_open(kr_decklink_setup *setup);
int kr_decklink_get_name(kr_decklink_info *info);

#ifdef __cplusplus
}
#endif

#endif
