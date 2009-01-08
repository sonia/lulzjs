#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");
require("System/Net/Ports");

var url = arguments.shift() || "meh.doesntexist.org/data/random/anonymous_manifesto";
var get = new RegExp("^(http://)?([^/]*)(.*)$", "i").exec(url);

var host = get[2];
var page = get[3] || "/";

var socket = new Socket;
if (!socket.connect(host, Ports.HTTP)) {
    die("Couldn't connect to the host.");
}

socket.sendLine([
    "GET "+page+" HTTP/1.1",
    "Host: "+host,
    "Connection: close",
    ""
]);

if (!socket.receiveLine().match(/OK/)) {
    die("The page couldn't be retrieved");
}

var header = /^\s*([\w\-]+)\s*:\s*(.*)$/;

var line;
var length;
while (line = socket.receiveLine()) {
    var match = header.exec(line);

    if (match[1].toLowerCase() == "content-length") {
        length = parseInt(match[2]);
    }
}

var content = socket.receive(length);

Console.writeLine(content);
