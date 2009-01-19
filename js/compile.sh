#! /bin/sh
mkdir -p /usr/include/js
mkdir -p /usr/lib

make -f Makefile.ref LIBDIR=lib JS_THREADSAFE=1
make -f Makefile.ref install DESTDIR= INCLUDEDIR=/usr/include/js LIBDIR=lib
make -f Makefile.ref clean

gcc -ljs -I/usr/include/js -DXP_UNIX -DJS_THREADSAFE -fPIC -c lulzjs/lulzjs.c -o lulzjs.lo
gcc -ljs -shared -Wl,-soname,liblulzjs.so -o liblulzjs.so lulzjs.lo -lc
cp -f liblulzjs.so /usr/lib/
cp -f lulzjs/lulzjs.h /usr/include/js
rm lulzjs.lo
rm liblulzjs.so

chmod a+rx /usr/include/js
chmod a+r  /usr/include/js/*
chmod a+rx /usr/lib/libjs.so
chmod a+rx /usr/lib/liblulzjs.so

