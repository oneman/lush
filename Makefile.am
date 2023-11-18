ACLOCAL_AMFLAGS = -I m4
if KR_IZ_US
  KR_DEV_TOOLS = tools/code/analysis/gather_structs tools/code/gen/bootstrap tools/code/gen tools/code/gen/tests
  KR_TESTS = tests
endif
if KR_ENABLE_TOOLS
  KR_TOOLS = tools
endif
SUBDIRS = $(KR_DEV_TOOLS) . lib daemon clients $(KR_TESTS) $(KR_TOOLS)
