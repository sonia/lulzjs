#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});
var panel  = new ncurses.Panel({x: 2, y: 2, width: 30, height: 10, border: true});
panel.printString("LOL", {x: 1, y: 1});
panel.getChar();
