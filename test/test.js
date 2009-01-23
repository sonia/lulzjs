#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});
screen.printString("LOL", {fg: ncurses.Colors.Red, at:ncurses.Attributes.Standout});
screen.getChar();
screen.printString("OMG", {x: 10, y: 10, bg: ncurses.Colors.Red});
screen.printChar(screen.getChar(), {x: 35, y: 10});
screen.getChar()
