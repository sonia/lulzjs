#! /usr/bin/env ljs
require("System/Console");

var LOL = Class.create({
    initialize: function (wat) {
        Program.wat = wat;

        Console.writeLine("Hello, I'm a thread. And Program.wat is "+Program.wat);
    }
});

var thread = new Thread(LOL);
thread.start('nigguz');

Console.writeLine(Program.wat);
Console.writeLine(Program.wat);

