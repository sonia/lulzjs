#! /usr/bin/env ljs
require("System/Thread");

var NewThread = Class.create(Thread, {
    _start: function (/* params passed with start */) {
        /* first thing executed with Thread.start*/
    }
});

var thread = new NewThread;
thread.start(/* params */);
