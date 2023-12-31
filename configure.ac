AC_INIT([kradradio], m4_esyscmd([scripts/kr_version.sh]), [kradradio@gmail.com])
AC_CONFIG_SRCDIR([lib/radio/radio.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign no-define subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
AC_USE_SYSTEM_EXTENSIONS
AM_MAINTAINER_MODE([enable])
LT_INIT
AM_SILENT_RULES([yes])
KR_LT_CURRENT=$PACKAGE_VERSION
KR_LT_REVISION=1
KR_LT_AGE=0
AC_SUBST(KR_LT_CURRENT)
AC_SUBST(KR_LT_REVISION)
AC_SUBST(KR_LT_AGE)
AC_SUBST(KR_GIT_VERSION, m4_esyscmd([git log -n 1 --format=%H]))

CFLAGS="-Wall $CFLAGS"
CXXFLAGS="-g -Wall -Wno-write-strings $CXXFLAGS"

AM_CONDITIONAL(KR_LINUX, true)
AM_CONDITIONAL(KR_ALSA, true)
AM_CONDITIONAL(KR_V4L2, true)

AC_ARG_ENABLE([optimize], AS_HELP_STRING([--enable-optimize],[Enable Optimize]),
 [optimize=yes],[optimize=no])
AS_IF([test "$optimize" = "yes"],[CFLAGS="$CFLAGS -O3 -mtune=native"])

AC_ARG_ENABLE([dev], AS_HELP_STRING([--enable-dev],[Enable developer tools]),
 [dev=yes],[dev=no])
AS_IF([test "$dev" = "yes"],[AC_DEFINE([KR_IZ_US],[1],[Enable dev tools])])
AM_CONDITIONAL([KR_IZ_US],[test "$dev" = "yes"])

AC_ARG_ENABLE([tools], AS_HELP_STRING([--enable-tools],[Enable tools]),
 [tools=yes],[tools=no])
AS_IF([test "$tools" = "yes"],[AC_DEFINE([KR_ENABLE_TOOLS],[1],[Enable tools])])
AM_CONDITIONAL([KR_ENABLE_TOOLS],[test "$tools" = "yes"])

AC_ARG_ENABLE([decklink], AS_HELP_STRING([--disable-decklink],[Disable Decklink]),
 [decklink=no],[decklink=yes])
AS_IF([test "$decklink" = "yes"],[AC_DEFINE([KR_DECKLINK],[1],[Disable Decklink])])
AM_CONDITIONAL([KR_DECKLINK],[test "$decklink" = "yes"])

AC_ARG_ENABLE([daala], AS_HELP_STRING([--enable-daala],[Enable daala]),
 [daala_enable=yes],[daala_enable=no])
AS_IF([test "$daala_enable" = "yes"],[AC_DEFINE([KR_ENABLE_DAALA],[1],[Enable daala])])
AM_CONDITIONAL([KR_ENABLE_DAALA],[test "$daala_enable" = "yes"])

AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(uuid, uuid_unparse_lower)
AC_CHECK_LIB(m, floorf)
AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(rt, clock_nanosleep)
AC_CHECK_LIB(cap, cap_get_proc)
AC_CHECK_LIB(z, deflate)
AC_CHECK_LIB(mp3lame, lame_init)

PKG_CHECK_MODULES([QUVI], [libquvi-0.9])
PKG_CHECK_MODULES([OPUS], [opus])
PKG_CHECK_MODULES([VORBIS], [vorbis,vorbisenc])
PKG_CHECK_MODULES([FLAC], [flac])
PKG_CHECK_MODULES([THEORA], [theora,theoraenc,theoradec])
PKG_CHECK_MODULES([VPX], [vpx])
PKG_CHECK_MODULES([X264], [x264])
PKG_CHECK_MODULES([AAC], [fdk-aac])

PKG_CHECK_MODULES([FREETYPE2], [freetype2])
PKG_CHECK_MODULES([CAIRO], [cairo,cairo-ft])
PKG_CHECK_MODULES([SAMPLERATE], [samplerate])
PKG_CHECK_MODULES([AVFORMAT], [libavformat])
PKG_CHECK_MODULES([AVCODEC], [libavcodec])
PKG_CHECK_MODULES([AVDEVICE], [libavdevice])
PKG_CHECK_MODULES([AVFILTER], [libavfilter])
PKG_CHECK_MODULES([AVRESAMPLE], [samplerate])
PKG_CHECK_MODULES([AVUTIL], [libavutil])
PKG_CHECK_MODULES([LIBSWSCALE], [libswscale])
PKG_CHECK_MODULES([TLS], [libssl])

AC_CHECK_LIB(turbojpeg, tjInitDecompress,[turbojpeg=yes],[turbojpeg=no])
AS_IF([test "$turbojpeg" = "yes"],[AC_DEFINE([HAVE_TURBOJPEG],[1],[Use turbojpeg])])
AM_CONDITIONAL([HAVE_TURBOJPEG],[test "$turbojpeg" = "yes"])

PKG_CHECK_MODULES([GTK], [gtk+-3.0], [gtk=yes], [gtk=no])
AS_IF([test "$gtk" = "yes"],[AC_DEFINE([HAVE_GTK],[1],[Use GTK])])
AM_CONDITIONAL([HAVE_GTK],[test "$gtk" = "yes"])

AC_CHECK_LIB(gif, DGifOpenFileName,[gif=yes],[gif=no])
AS_IF([test "$gif" = "yes"],[AC_DEFINE([HAVE_GIF],[1],[Use GIF])])
AM_CONDITIONAL([HAVE_GIF],[test "$gif" = "yes"])

PKG_CHECK_MODULES([ALSA],[alsa],[alsa=yes],[alsa=no])
AS_IF([test "$alsa" = "yes"],[AC_DEFINE([HAVE_ALSA],[1],[Use ALSA])])
AM_CONDITIONAL([HAVE_ALSA],[test "$alsa" = "yes"])

AS_IF([test "$daala_enable" = "yes"],[PKG_CHECK_MODULES([DAALA],[daaladec,daalaenc],[daala=yes],[daala=no])])

AS_IF([test "$daala_enable" = "yes"],[AS_IF([test "$daala" = "yes"],[nnv="Next Next Video!"],[nnv="Sad Monty :("])],[nnv="Sad Monty :("])

AS_IF([test "$daala" = "yes"],[nnv="Next Next Video!"],[nnv="Sad Monty :("])

AS_IF([test "$daala" = "yes"],[AC_DEFINE([HAVE_DAALA],[1],[Use Daala])])
AM_CONDITIONAL([HAVE_DAALA],[test "$daala" = "yes"])

PKG_CHECK_MODULES([JACK],[jack],[jack=yes],[jack=no])
AS_IF([test "$jack" = "yes"],[AC_DEFINE([HAVE_JACK],[1],[Use JACK])])
AM_CONDITIONAL([HAVE_JACK],[test "$jack" = "yes"])

PKG_CHECK_MODULES([NCURSES],[ncurses,panel,menu,form],[ncurses=yes],[ncurses=no])
AS_IF([test "$ncurses" = "yes"],[AC_DEFINE([HAVE_NCURSES],[1],[Use NCURSES])])
AM_CONDITIONAL([HAVE_NCURSES],[test "$ncurses" = "yes"])

PKG_CHECK_MODULES([LIBUDEV],[libudev],[libudev=yes],[libudev=no])
AS_IF([test "$libudev" = "yes"],[AC_DEFINE([HAVE_LIBUDEV],[1],[Use libudev])])
AM_CONDITIONAL([HAVE_LIBUDEV],[test "$libudev" = "yes"])

PKG_CHECK_MODULES([XKBCOMMON],[xkbcommon],[xkbcommon=yes],[xkbcommon=no])
AS_IF([test "$xkbcommon" = "yes"],[AC_DEFINE([HAVE_XKBCOMMON],[1],[Use xkbcommon])])
AM_CONDITIONAL([HAVE_XKBCOMMON],[test "$xkbcommon" = "yes"])

PKG_CHECK_MODULES([WAYLAND],[wayland-client],[wayland=yes],[wayland=no])
AS_IF([test "$wayland" = "yes"],[AC_DEFINE([HAVE_WAYLAND],[1],[Use Wayland])])
AM_CONDITIONAL([HAVE_WAYLAND],[test "$wayland" = "yes"])

PKG_CHECK_MODULES([X11],[x11,x11-xcb,xcb-image,xcb-shm],[x11=yes],[x11=no])
AS_IF([test "$x11" = "yes"],[AC_DEFINE([HAVE_X11],[1],[Use X11])])
AM_CONDITIONAL([HAVE_X11],[test "$x11" = "yes"])

PKG_CHECK_MODULES([XMMS2],[xmms2-client],[xmms2=yes],[xmms2=no])
AS_IF([test "$xmms2" = "yes"],[AC_DEFINE([HAVE_XMMS2],[1],[Use XMMS2])])
AM_CONDITIONAL([HAVE_XMMS2],[test "$xmms2" = "yes"])

LT_LIB_M

AC_PROG_CC
AC_PROG_CXX

AC_CONFIG_FILES([Makefile lib/Makefile daemon/Makefile clients/Makefile
                 tests/Makefile tools/code/analysis/gather_structs/Makefile
                 tools/code/gen/bootstrap/Makefile tools/code/gen/Makefile
                 tools/code/gen/tests/Makefile tools/Makefile demo/Makefile])

AC_OUTPUT

AC_MSG_RESULT([
------------------------------------------------------------------------
           Demo: Build Configuration

        Version: $PACKAGE_VERSION
             CC: $CC
         CFLAGS: $CFLAGS

          Tools: $tools
      Dev Tools: $dev

           ALSA: $alsa
          Daala: $daala $nnv
       Decklink: $decklink
            gif: $gif
            gtk: $gtk
           JACK: $jack
        ncurses: $ncurses
      turbojpeg: $turbojpeg
        libudev: $libudev
        Wayland: $wayland
            X11: $x11
          XMMS2: $xmms2
-------------------------------------------------------------------------
])

echo "Type \"make\" to compile";
echo "Type \"make install\" (as root) to install";
echo "Type \"make uninstall\" (as root) to uninstall";
