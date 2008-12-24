#! /usr/bin/ljs
#include <System>

System.Console.writeLine(new System.IO.File("file.js", "r").readToEnd());
