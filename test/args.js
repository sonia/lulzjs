#! /usr/bin/ljs
require("System/Console");

Console.writeLine("Program.name: "+this.name);

arguments.each(function (value, index) {
    Console.writeLine("{index}: {value}".format({
        index: index,
        value: value
    }));
});
