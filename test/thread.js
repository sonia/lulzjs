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
thread.start('nigguz');

//setTimeout(function(){Console.writeLine(Program.wat)}, 5000);
