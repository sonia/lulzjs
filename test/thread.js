#! /usr/bin/env ljs
require("System/Console");
require("System/Thread");

var LOL = Class.create({
    initialize: function (wat) {
        Program.wat = wat;
        Console.writeLine("IN: "+Program.wat);
    }
});

var thread = new Thread(LOL);
Console.writeLine(thread);
thread.start('nigguz');
Console.writeLine("OUT: "+Program.wat);

setTimeout('Console.writeLine("1: "+Program.wat)', 5000);
setTimeout('Console.writeLine("2: "+thread.__object.initialize)', 3000);
