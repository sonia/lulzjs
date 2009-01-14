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
    initialize: function (wat) {
        Program.wat = wat;

        Console.writeLine("Hello, I'm a thread. And Program.wat is "+Program.wat+" and it's "+new Date);
    }
});

setTimeout("Console.writeLine('lolnigger');", 2000);

/*for (var i = 0; i < 10; i++) {
    var thread = new Thread(LOL);
    thread.start(i);
}*/

sleep(5);

