#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP/Simple");

var url  = arguments.shift();
var data = arguments.shift();

var output = HTTP.Post(url, data.parseQuery());
Console.writeLine(output);
