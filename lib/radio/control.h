#if !defined(_radio_control_H)
# define _radio_control_H (1)

#include <krad/app/common.h>

void krad_radio_launch(char *sysname);
int krad_radio_destroy(char *sysname);
int krad_radio_running(char *sysname);
char *krad_radio_running_stations(void);

#endif
