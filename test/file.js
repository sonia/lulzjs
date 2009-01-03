#! /usr/bin/ljs
require("System");

System.Console.writeLine(new System.IO.File("file.js", "r").readToEnd());
