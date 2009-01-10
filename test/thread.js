#! /usr/bin/env ljs
require("System/Console");
require("System/Thread");

var LOL = Class.create(Thread, {
    __start: function (wat) {
        Program.wat = wat;
        Console.writeLine(Program.wat);
    }
});

var thread = new LOL;

Console.writeLine(Program.wat);
thread.start(':O');

for (var i = 0; i < 10000; i++);

Console.writeLine(Program.wat);
