#! /usr/bin/ljs
require("System/Console");
require("System/IO/File");

Console.writeLine(new File("test/lol.js", "r").readLine());
