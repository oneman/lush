include $(top_srcdir)/common.mk

codegen_bootstrap_CPPFLAGS =
codegen_bootstrap_CPPFLAGS += '-I../'

info_gen_CPPFLAGS = '-I../analysis/gather_structs/'
info_gen_CPPFLAGS += '-Ibootstrap/'
info_gen_CPPFLAGS += '-I../'

noinst_PROGRAMS = codegen_pre info_gen meta_gen wrap_gen mime_gen limits_gen

codegen_pre$(EXEEXT): ; test -s bootstrap/bootstrapped.c && test -s bootstrap/bootstrapped.h || ./bootstrap/codegen_bootstrap ../../../lib "" "" bootstrap/

meta_gen_CPPFLAGS = ${info_gen_CPPFLAGS}
meta_gen_SOURCES = ../analysis/gather_structs/structs_gathering.c \
  bootstrap/codegen_bootstrap_utils.c \
  bootstrap/bootstrapped.c \
  meta_gen.c

info_gen_SOURCES = ../analysis/gather_structs/structs_gathering.c \
  info_gen.c \
  bootstrap/codegen_bootstrap_utils.c \
  bootstrap/bootstrapped.c

wrap_gen_CPPFLAGS = ${info_gen_CPPFLAGS}
wrap_gen_SOURCES = ../analysis/gather_structs/structs_gathering.c \
  bootstrap/codegen_bootstrap_utils.c \
  bootstrap/bootstrapped.c \
  wrap_gen.c

mime_gen_SOURCES = mime_gen.c
limits_gen_SOURCES = limits_gen.c
