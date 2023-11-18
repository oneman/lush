include $(top_srcdir)/common.mk

codegen_bootstrap_CPPFLAGS = '-I../../analysis/gather_structs/'
codegen_bootstrap_CPPFLAGS += '-I../../'

noinst_PROGRAMS = codegen_bootstrap

codegen_bootstrap_SOURCES = ../../analysis/gather_structs/structs_gathering.c \
  codegen_bootstrap.c \
  codegen_bootstrap_utils.c