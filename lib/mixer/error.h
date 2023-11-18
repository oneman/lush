#if !defined(_mixer_error_H)
# define _mixer_error_H (1)

#define KR_ERR_MIXER_CROSSCRANKED -1000
#define KR_ERR_MIXER_SOUNDBLASTER16 -16

#include <krad/app/fail.h>

/*static const char *kr_mixer_strerr_idx[] = {
  [KR_ERRIDX(MIXER_CROSSCRANKED)] = "Mixer: Crosscranked",
  [KR_ERRIDX(MIXER_SOUNDBLASTER16)] = "Mixer: Soundblaster 16 detected.."
};*/

#ifdef KR_COM_STRERR_IDX
#undef KR_COM_STRERR_IDX
#endif

#define KR_COM_STRERR_IDX kr_mixer_strerr_idx

#endif

