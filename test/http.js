#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP/Simple");

var content = HTTP.Get(arguments.shift());

Console.writeLine(content.length+":\n"+content);
