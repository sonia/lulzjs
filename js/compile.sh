#! /bin/sh
mkdir -p /usr/include/js
mkdir -p /usr/lib

make -f Makefile.ref LIBDIR=lib JS_THREADSAFE=1
make -f Makefile.ref install DESTDIR= INCLUDEDIR=/usr/include/js LIBDIR=lib

chmod a+rx /usr/include/js
chmod a+r  /usr/include/js/*
chmod a+rx /usr/lib/libjs.so

make -f Makefile.ref clean
