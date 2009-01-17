#! /bin/sh
mkdir -p /usr/lib/lulzjs/js
make -f Makefile.ref LIBDIR=lib JS_THREADSAFE=1
make -f Makefile.ref install DESTDIR= INCLUDEDIR=/usr/lib/lulzjs/js LIBDIR=lib/lulzjs/js
chmod -R a+rx /usr/lib/lulzjs
