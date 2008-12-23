#! /bin/sh
gcc -ljs -DXP_UNIX -fPIC -o System.o -c System.c
gcc -shared -Wl,-soname,System.so -o System.so System.o -lc
gcc -ljs -DXP_UNIX -fPIC -o IO.o -c IO/IO.c
gcc -shared -Wl,-soname,IO.so -o IO.so IO.o -lc
