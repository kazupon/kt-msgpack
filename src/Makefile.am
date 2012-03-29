
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
	gen/types.hpp

# -version-info CURRENT:REVISION:AGE
libktmsgpack_la_LDFLAGS = -version-info 0:0:0

EXTRA_DIST = kt.thrift

