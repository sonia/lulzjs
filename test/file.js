#! /usr/bin/ljs
require("System/Console");
require("System/IO/File");

Console.writeLine(new File("file.js", "r").readToEnd());
