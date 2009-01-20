#! /usr/bin/env ljs
require("System/Console");

// WARNING, threads still need some (a lot) of work

var LOL = Class.create({
    initialize: function (a, b) {
        sleep(1);
        this.__return = a + b;
    }
});

var thread = new Thread(LOL);
thread.start(2, 4);
Console.writeLine("ID: "+thread.__id);
Console.writeLine(thread.join());
