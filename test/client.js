#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

var socket = new Socket;
socket.connect("localhost", 2707);

socket.sendLine("lessbigotedword...");
Console.writeLine("Server: "+(socket.receiveLine()));
