#! /usr/bin/env ljs
require("System/Console");
require("System/Net/Socket");

if (arguments.length < 1)
    die("You must pass at least a hostname to resolve.");

arguments.each(function (host) {
    var resolved;

    try {
        resolved = Socket.getHostByName(host);
    }
    catch (e) {
        resolved = "couldn't resolve the host name.";
    }

    Console.writeLine("{0}: {1}".format([host, resolved]));
});
