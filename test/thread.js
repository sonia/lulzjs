#! /usr/bin/env ljs
require("System/Console");
require("System/Thread");

var LOL = Class.create({
    initialize: function (wat) {
        Program.wat = wat;
        Console.writeLine(Program.wat);
    }
});

var thread = new Thread(LOL);

Console.writeLine(Program.wat);
thread.start('D:');

setTimeout('Console.writeLine("1: "+Program.wat)', 5000);
setTimeout('Console.writeLine("2: tette")', 3000);
