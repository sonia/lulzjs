#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP/Simple");

Console.writeLine(HTTP.Get(arguments.shift()));
