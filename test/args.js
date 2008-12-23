#! /usr/bin/ljs
#include <System>

for (var i = 0; i < arguments.length; i++) {
    System.IO.Console.writeLine(i+": "+arguments[i]);
}
