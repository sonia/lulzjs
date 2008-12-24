#! /usr/bin/ljs
#include <System>

arguments.each(function (value, index) {
    System.Console.writeLine("{index}: {value}".format({
        index: index,
        value: value
    }));
});
