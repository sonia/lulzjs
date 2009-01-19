#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.listen(null, 2707);
var client = socket.accept();

Console.writeLine("Client: "+(client.receiveBytes(4)));
client.sendBytes(new Bytes([0x61,0x00]));
