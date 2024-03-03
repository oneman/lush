#if !defined(_app_debug_H)
# define _app_debug_H (1)

#include <krad/mem/mem.h>

#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#define KR_DONE 666
#define KR_LOG_ERRORS 1
#define KR_SNAFU 2
#define KR_FUBAR 3

//#define KR_DEVCON KR_DONE
//#define KR_DEVCON KR_LOG_ERRORS
#define KR_DEVCON KR_SNAFU
//#define KR_DEVCON KR_FUBAR

#ifndef KR_DEVCON
#define KR_DEVCON KR_LOG_ERRORS
#endif

void kr_print_noop(char* format, ...);
void kr_print(char* format, ...);
void kr_print_err(char* format, ...);
void failfast(char* format, ...);

#if KR_DEVCON == KR_DONE
#define printkd kr_print_noop
#define printk kr_print_noop
#define printke kr_print_noop
#elif KR_DEVCON == KR_LOG_ERRORS
#define printkd kr_print_noop
#define printk kr_print_noop
#define printke kr_print_err
#elif KR_DEVCON == KR_SNAFU
#define printkd kr_print_noop
#define printk kr_print
#define printke kr_print_err
#elif KR_DEVCON == KR_FUBAR
#define printkd kr_print
#define printk kr_print
#define printke kr_print_err
#else
#define printkd failfast
#define printk failfast
#define printke failfast
#endif

#include <krad/cnake/header.h>
#endif
