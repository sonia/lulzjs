CC         = gcc
CXX        = g++
BINDIR     = /usr/bin
LJS_LIBDIR = /usr/lib/lulzjs
CFLAGS    += -DXP_UNIX -D__LJS_LIBRARY_PATH__="\"${LJS_LIBDIR}/\""
LDFLAGS   += -ljs

CORE         = src/core/main.o src/core/Core.o src/core/Misc.o src/core/Preprocessor.o
CORE_CFLAGS  = ${CFLAGS}
CORE_LDFLAGS = ${LDFLAGS} -ldl

LIB_SYSTEM         = src/lib/System/System.o src/lib/System/IO/IO.o
LIB_SYSTEM_CFLAGS  = ${CFLAGS}
LIB_SYSTEM_LDFLAGS = ${LDFLAGS}

all: core libcore libsystem

core: $(CORE)
	${CC} ${CORE_LDFLAGS} ${CORE_CFLAGS} ${CORE} -o ljs

core_install:
	cp ljs ${BINDIR}/

libcore:

libcore_install:
	mkdir -p ${LJS_LIBDIR}/Core
	cp src/core/Core/init.js ${LJS_LIBDIR}/Core/init.js
	
libsystem: $(LIB_SYSTEM)

$(LIB_SYSTEM): $(LIB_SYSTEM:.o=.c)
	${CC} ${LIB_SYSTEM_LDFLAGS} ${LIB_SYSTEM_CFLAGS} -fPIC -c $*.c -o $*.lo
	${CC} -shared -Wl,-soname,$*.so -o $*.o $*.lo -lc

libsystem_install:
	mkdir -p ${LJS_LIBDIR}/System
	mkdir -p ${LJS_LIBDIR}/System/IO
	cp src/lib/System/init.js       ${LJS_LIBDIR}/System/init.js
	cp src/lib/System/System.o     ${LJS_LIBDIR}/System/System.so
	cp src/lib/System/IO/IO.o      ${LJS_LIBDIR}/System/IO/IO.so
	cp src/lib/System/IO/Console.js ${LJS_LIBDIR}/System/IO/Console.js

install: core_install libcore_install libsystem_install

clean:
	rm -f src/core/*.o;
	rm -f src/lib/System/*.*o; rm -f src/lib/System/IO/*.*o
