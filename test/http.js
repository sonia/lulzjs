#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Protocol/HTTP/Simple");
require("System/IO/File");

var url = arguments.shift();
var out = arguments.shift();

var content = HTTP.Get(url);

if (out) {
    Console.writeLine(content);
}
else {
    var file = new File(File.baseName(url), "w");
    if (Object.is(content, Bytes)){
        file.writeBytes(content);
    }
    else {
        file.write(content);
    }
}
