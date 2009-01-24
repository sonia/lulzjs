#! /usr/bin/env ljs
require("ncurses");

var screen = new ncurses.Screen({buffering: ncurses.Buffering.Raw});

var panels = []
panels.push(new ncurses.Panel({x: 2, y: 2, width: 30, height: 10, border: true}));
panels.push(new ncurses.Panel({x: 4, y: 5, width: 30, height: 10, border: true}));
panels[0].printString("LOL", {x: 1, y: 1});
panels[1].printString("PENIS", {x: 10, y: 4});
panels[0].toTop();
panels[0].getChar();
panels[0].below.getChar();
