#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});
var panel  = new ncurses.Panel({x: 0, y: 0, width: 30, height: 10, border: true});
screen.printString(panel.__window.getChar());
screen.refresh();
screen.getChar();
