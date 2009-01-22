VERSION = 0.1

CC         = gcc
CXX        = g++
BINDIR     = /usr/bin
LJS_LIBDIR = /usr/lib/lulzjs
CFLAGS     = -Os -DXP_UNIX -DJS_THREADSAFE -D__LIB_VERSION__="\"${VERSION}\"" -I/usr/include/js -I./src
LDFLAGS    = -ljs -llulzjs

ifdef DEBUG
CFLAGS += -g
endif

ifdef DDEBUG
CFLAGS += -DDEBUG -g
endif

LIB_DIR     = src
LIB         = ${LIB_DIR}/ncurses.o ${LIB_DIR}/Screen.o
LIB_CFLAGS  = ${CFLAGS}
LIB_LDFLAGS = ${LDFLAGS} -lncurses

all: lib

lib: $(LIB)

$(LIB) : $(LIB:.o=.c)
	${CC} ${LIB_CFLAGS} -fPIC -c $*.c -o $*.lo
	${CC} ${LIB_LDFLAGS} -shared -Wl,-soname,`basename $*`.so -o $*.o $*.lo -lc

install: lib_install

lib_install: lib
	mkdir -p ${LJS_LIBDIR}/ncurses
########
	cp -f  ${LIB_DIR}/init.js		${LJS_LIBDIR}/ncurses/init.js
	cp -f  ${LIB_DIR}/ncurses.o		${LJS_LIBDIR}/ncurses/ncurses.so
	cp -f  ${LIB_DIR}/ncurses.js	${LJS_LIBDIR}/ncurses/ncurses.js
	cp -f  ${LIB_DIR}/Screen.o		${LJS_LIBDIR}/ncurses/Screen.so
	cp -f  ${LIB_DIR}/Screen.js		${LJS_LIBDIR}/ncurses/Screen.js
#######
	chmod -R a+rx ${LJS_LIBDIR}/ncurses

clean:
	find src|egrep "\.l?o"|xargs rm -f
