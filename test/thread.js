#! /usr/bin/env ljs
require("System/Console");

function setTimeout (expression, timeout) {
    var thread = new Thread(Class.create({
        initialize: function (expression, timeout) {
            Console.writeLine("NIGGER");
            sleep(timeout/1000);
            eval(expression);
        }
    }), true);

    thread.start(expression, timeout);
}

var LOL = Class.create({
    initialize: function (a, b) {
        return a + b;
    }
});

var thread = new Thread(LOL);
thread.start(2, 4);
Console.writeLine("NIGGER");
Console.writeLine(thread.join());
