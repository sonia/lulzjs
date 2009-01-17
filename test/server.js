#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.listen(null, 2707);
var client = socket.accept();

Console.writeLine(client.receiveLine());
client.sendLine("NO U");
