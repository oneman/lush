#if !defined(_adapter_monitor_H)
# define _adapter_monitor_H (1)

/*
 * TODO consider inotify monitor for:
 *    wayland: $XDG_RUNTIME_DIR/wayland* created
 *      jackd: /dev/shm/jack_* created
 *
 */

typedef struct kr_adapter_monitor kr_adapter_monitor;

int kr_adapter_monitor_destroy(kr_adapter_monitor *monitor);
kr_adapter_monitor *kr_adapter_monitor_create();
void kr_adapter_monitor_wait(kr_adapter_monitor *monitor, int ms);

#endif
