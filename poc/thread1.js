#! /usr/bin/env ljs
require("System/Thread");

var NewThread = Class.create({
    initialize: function (/* params passed with start */) {
        /* first thing executed with Thread.start*/
    }
});

var thread = new Thread(NewThread);
thread.start(/* params */);
