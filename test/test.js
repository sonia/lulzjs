#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});
screen.printLine("LOL");
screen.getChar();
screen.printLine("OMG", 10, 10);
screen.printLine(chr(screen.getChar()), 35, 10);
screen.getChar()
