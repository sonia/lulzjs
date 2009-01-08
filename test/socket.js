#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.connect("meh.doesntexist.org", 80);

socket.sendLine([
    "GET / HTTP/1.1",
    "Host: meh.doesntexist.org",
    ""
]);

var line;
while (line = socket.receiveLine()); 

while (line = socket.receiveLine()) {
//    Console.writeLine(line);
}

Console.writeLine(line[0]+": "+line.charCodeAt(0));
