#! /usr/bin/env ljs
require("System/Console");

var LOL = Class.create({
    initialize: function (a, b) {
        this.__return = a + b;
    }
});

var thread = new Thread(LOL, true);
thread.start(2, 4);
Console.writeLine("ID: "+thread.__id);

sleep(2);
Console.writeLine(thread.__return);
