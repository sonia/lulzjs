#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});
var win    = new ncurses.Window({x: 2, y: 2, width: 40, height: 10});
win.printString("LOLNIGGERS", {x: 10, y: 4, fg: ncurses.Colors.Blue});
win.printChar(win.getChar(), {x: 2, y: 2})
win.getChar();
