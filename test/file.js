#! /usr/bin/ljs
require("System");

var file = new System.IO.File("file.js", "r");

System.Console.writeLine(file.readToEnd());
