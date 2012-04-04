
plugindir = $(libexecdir)

plugin_LTLIBRARIES = libktmsgpack.la

libktmsgpack_la_SOURCES = \
	kt-msgpack.cc \
	gen/KyotoTycoonService.cpp

noinst_HEADERS = \
	gen/KyotoTycoonService.hpp \
	gen/KyotoTycoonService_client.hpp \
	gen/KyotoTycoonService_server.hpp \
	gen/client.hpp \
	gen/server.hpp \
	gen/types.hpp \
	cmdcommon.h \
	myscript.h

# -version-info CURRENT:REVISION:AGE
libktmsgpack_la_LDFLAGS = -version-info 0:0:0

bin_PROGRAMS = ktmpremotetest
ktmpremotetest_SOURCES = ktmpremotetest.cc

EXTRA_DIST = kt.thrift

