#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

if (arguments.length < 1)
    die(Console.errorLine("You must pass at least a hostname to resolve."));

arguments.each(function (host) {
    Console.writeLine("{0}: {1}".format([
        host,
        Socket.getHostByName(arguments[1])
    ]));
});
