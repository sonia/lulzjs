#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});
screen.printLine("LOL", {fg: ncurses.Colors.Red, at:ncurses.Attributes.Standout});
screen.getChar();
screen.printLine("OMG", 10, 10, {bg: ncurses.Colors.Red});
screen.printLine(chr(screen.getChar()), 35, 10);
screen.getChar()
