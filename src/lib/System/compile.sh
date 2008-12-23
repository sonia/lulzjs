#! /bin/sh
LJS_LIBDIR=/usr/lib/lulzjs

gcc -ljs -DXP_UNIX -fPIC -o System.o -c System.c
gcc -shared -Wl,-soname,System.so -o System.so System.o -lc
gcc -ljs -DXP_UNIX -fPIC -o IO.o -c IO/IO.c
gcc -shared -Wl,-soname,IO.so -o IO.so IO.o -lc

mkdir -pv ${LJS_LIBDIR}/System/IO

cp -v init.js ${LJS_LIBDIR}/System/
mv -v System.so ${LJS_LIBDIR}/System/
mv -v IO.so ${LJS_LIBDIR}/System/IO/
cp -v IO/Console.js ${LJS_LIBDIR}/System/IO/

chmod -R a+rx ${LJS_LIBDIR}
