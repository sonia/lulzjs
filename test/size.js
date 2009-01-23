#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});

(screen.onResize = function () {
    screen.printString("Width", {x: 0, y: 0, fg: ncurses.Colors.Red, at: ncurses.Attributes.Underline});
    screen.printString(" : "+screen.Size.Width+"   ");
    screen.printString("Height", {x: 0, y: 1, fg: ncurses.Colors.Red, at: ncurses.Attributes.Underline});
    screen.printString(": "+screen.Size.Height+"   ");
})();

while (screen.getChar() != ord('q'));
