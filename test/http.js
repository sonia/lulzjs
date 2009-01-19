#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP/Simple");
require("System/IO/File");

var content = HTTP.Get(arguments.shift());

var file = new File("omg", "w");
file.writeBytes(content.content);
