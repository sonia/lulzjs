VERSION = 0.1.7
SPIDERMONKEY_HEADERS = /usr/include/js
SPIDERMONKEY_LIB     = -ljs

CC         = gcc
CXX        = g++
BINDIR     = /usr/bin
LJS_LIBDIR = /usr/lib/lulzjs
CFLAGS     = -DXP_UNIX -D__LJS_LIBRARY_PATH__="\"${LJS_LIBDIR}\"" -D__LJS_VERSION__="\"${VERSION}\"" -I${SPIDERMONKEY_HEADERS}
LDFLAGS    = ${SPIDERMONKEY_LIB} -lpthread

ifdef DEBUG
CFLAGS += -g
endif

ifdef DDEBUG
CFLAGS += -DDEBUG -g
endif

CORE         = src/core/main.o src/core/Core.o src/core/Misc.o src/core/Interactive.o src/core/Hash.o
CORE_CFLAGS  = ${CFLAGS}
CORE_LDFLAGS = ${LDFLAGS} -ldl -lreadline -lncurses

LIB_SYSTEM = \
	src/lib/System/System.o \
	src/lib/System/Thread/Thread.o \
	src/lib/System/IO/IO.o src/lib/System/IO/Stream/Stream.o src/lib/System/IO/File/File.o \
	src/lib/System/Net/Net.o src/lib/System/Net/Socket/Socket.o

LIB_SYSTEM_CFLAGS  = ${CFLAGS}
LIB_SYSTEM_LDFLAGS = ${LDFLAGS}

all: ljs libcore libsystem

ljs: $(CORE)
	${CC} ${CORE_LDFLAGS} ${CORE_CFLAGS} ${CORE} -o ljs

core_install:
	mkdir -p ${LJS_LIBDIR}
	cp -f ljs ${BINDIR}/

libcore:

libcore_install:
	mkdir -p ${LJS_LIBDIR}
	cp -rf src/core/Core ${LJS_LIBDIR}/Core
	
libsystem: $(LIB_SYSTEM)

$(LIB_SYSTEM): $(LIB_SYSTEM:.o=.c)
	${CC} ${LIB_SYSTEM_LDFLAGS} ${LIB_SYSTEM_CFLAGS} -fPIC -c $*.c -o $*.lo
	${CC} -shared -Wl,-soname,`basename $*`.so -o $*.o $*.lo -lc

libsystem_install:
	mkdir -p ${LJS_LIBDIR}
	mkdir -p ${LJS_LIBDIR}/System
	mkdir -p ${LJS_LIBDIR}/System/Thread
	mkdir -p ${LJS_LIBDIR}/System/Console
	mkdir -p ${LJS_LIBDIR}/System/IO
	mkdir -p ${LJS_LIBDIR}/System/IO/Stream
	mkdir -p ${LJS_LIBDIR}/System/IO/File
	mkdir -p ${LJS_LIBDIR}/System/Net
	mkdir -p ${LJS_LIBDIR}/System/Net/Socket
	mkdir -p ${LJS_LIBDIR}/System/Net/Ports
########
	cp -f src/lib/System/init.js				${LJS_LIBDIR}/System/init.js
	cp -f src/lib/System/System.o				${LJS_LIBDIR}/System/System.so
########
	cp -f src/lib/System/Thread/init.js			${LJS_LIBDIR}/System/Thread/init.js
	cp -f src/lib/System/Thread/Thread.o		${LJS_LIBDIR}/System/Thread/Thread.so
########
	cp -f src/lib/System/IO/init.js				${LJS_LIBDIR}/System/IO/init.js
	cp -f src/lib/System/IO/IO.o				${LJS_LIBDIR}/System/IO/IO.so
########
	cp -f src/lib/System/IO/Stream/init.js		${LJS_LIBDIR}/System/IO/Stream/init.js
	cp -f src/lib/System/IO/Stream/Stream.o		${LJS_LIBDIR}/System/IO/Stream/Stream.so
	cp -f src/lib/System/IO/Stream/Stream.js	${LJS_LIBDIR}/System/IO/Stream/Stream.js
########
	cp -f src/lib/System/IO/File/init.js		${LJS_LIBDIR}/System/IO/File/init.js
	cp -f src/lib/System/IO/File/File.o			${LJS_LIBDIR}/System/IO/File/File.so
	cp -f src/lib/System/IO/File/File.js		${LJS_LIBDIR}/System/IO/File/File.js
########
	cp -f src/lib/System/Console/init.js		${LJS_LIBDIR}/System/Console/init.js
	cp -f src/lib/System/Console/Console.js		${LJS_LIBDIR}/System/Console/Console.js
#######
	cp -f src/lib/System/Net/init.js			${LJS_LIBDIR}/System/Net/init.js
	cp -f src/lib/System/Net/Net.o				${LJS_LIBDIR}/System/Net/Net.so
#######
	cp -f src/lib/System/Net/Socket/init.js		${LJS_LIBDIR}/System/Net/Socket/init.js
	cp -f src/lib/System/Net/Socket/Socket.o	${LJS_LIBDIR}/System/Net/Socket/Socket.so
	cp -f src/lib/System/Net/Socket/Socket.js	${LJS_LIBDIR}/System/Net/Socket/Socket.js
#######
	cp -f src/lib/System/Net/Ports/init.js		${LJS_LIBDIR}/System/Net/Ports/init.js
	cp -f src/lib/System/Net/Ports/Ports.js      ${LJS_LIBDIR}/System/Net/Ports/Ports.js

libsystem_uninstall:

install: all core_install libcore_install libsystem_install
	chmod -R a+rx ${LJS_LIBDIR}
	chmod a+rx ${BINDIR}/ljs

uninstall:
	rm -f ${BINDIR}/ljs
	rm -rf ${LJS_LIBDIR}
	

clean:
	rm -f ljs;
	find .|egrep "\.l?o"|xargs rm -f

