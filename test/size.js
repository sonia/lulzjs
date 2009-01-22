#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});

(screen.onResize = function () {
    screen.printLine("Width: {0}\nHeight: {1}".format([
        screen.Size.Width, screen.Size.Height]), 0, 0,
        {fg: ncurses.Colors.Red, at: ncurses.Attributes.Reverse}
    );
})();

while (screen.getChar() != ord('q'));
