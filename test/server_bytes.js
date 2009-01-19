#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.listen(null, 2707);

var client = socket.accept();
var bytes = client.receiveBytes(4);
Console.writeLine("Client: "+bytes);
client.sendBytes(new Bytes([0x61,0x00]));
Console.writeLine("DONE :O "+client);
