#if !defined(_audio_types_H)
# define _audio_types_H (1)

#define KR_AUDIO_NCHANNELS 8
#define KR_EQ_MAX_BANDS 32

typedef enum {
  KR_AFX_NONE,
  KR_AFX_EQ,
  KR_AFX_LOWPASS,
  KR_AFX_HIGHPASS,
  KR_AFX_ANALOG,
  KR_AFX_VOLUME
} kr_afx_effect_type;

typedef enum {
  KR_AFX_DB = 1,
  KR_AFX_BW,
  KR_AFX_HZ,
  KR_AFX_DRIVE,
  KR_AFX_BLEND
} kr_afx_effect_control;

typedef struct kr_eq_info kr_eq_info;
typedef struct kr_eq_band_info kr_eq_band_info;
typedef struct kr_lowpass_info kr_lowpass_info;
typedef struct kr_highpass_info kr_highpass_info;
typedef struct kr_pass_info kr_pass_info;
typedef struct kr_analog_info kr_analog_info;
typedef struct kr_volume_info kr_volume_info;
typedef struct kr_meter_info kr_meter_info;
typedef struct kr_afx_info kr_afx_info;

struct kr_eq_band_info {
  float db; /* -50.0, 20.0, 0.0 */
  float bw; /* 0.5, 4.0, 1.0 */
  float hz; /* 20.0, 20000.0 */
};

struct kr_eq_info {
  kr_eq_band_info band[KR_EQ_MAX_BANDS];
};

struct kr_lowpass_info {
  float bw; /* 0.66, 4.0, 0.66 */
  float hz; /* 20.0, 20000.0, 20000.0 */
};

struct kr_highpass_info {
  float bw; /* 0.66, 4.0, 0.66 */
  float hz; /* 20.0, 20000.0, 20.0 */
};

struct kr_pass_info {
  kr_lowpass_info low;
  kr_highpass_info high;
};

struct kr_volume_info {
  float gain; /* -120.0, 120.0, 0.0 */
  float fader; /* -120.0, 120.0, 0.0 */
};

struct kr_analog_info {
  float drive; /* 0.0, 10.0, 0.0 */
  float blend; /* -10.0, 10.0, 0.0 */
};

struct kr_meter_info {
  float avg; /* -120.0, 120.0, -120.0 */
  float peak; /* -120.0, 120.0, -120.0 */
};

struct kr_afx_info {
  kr_pass_info pass; /* opt */
  kr_analog_info analog; /* opt */
  kr_eq_info eq; /* opt */
  kr_volume_info volume; /* opt */
  kr_meter_info meter; /* opt */
};

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
