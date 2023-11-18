#define KR_VERSION 37

#ifndef KR_GIT_VERSION
#define KR_GIT_VERSION ""
#endif
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define KR_VERSION_STR STR(KR_VERSION)
#define KR_VERSION_STR_FULL "Krad Radio Version " KR_VERSION_STR " " KR_GIT_VERSION
