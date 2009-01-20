#! /usr/bin/env ljs
require("System/Console");
require("System/Crypt/SHA1");

if (arguments.length < 1) {
    die("You have to pass at least a string to transform into SHA1");
}

arguments.each(function (str) {
    Console.writeLine(str+": "+new SHA1(str));
});
