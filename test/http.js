#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP");

var http = new HTTP.Request("http://meh.doesntexist.org");
Console.writeLine(http.response.content);
