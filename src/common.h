/****************************************************************************
* This file is part of lulzJS-ncurses                                       *
* Copyleft meh.                                                             *
*                                                                           *
* lulzJS-ncurses is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS-ncurses is distributed in the hope that it will be useful.         *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS-ncurses.  If not, see <http://www.gnu.org/licenses/>.   *
****************************************************************************/

#ifndef _LULZJS_NCURSES_COMMON_H
#define _LULZJS_NCURSES_COMMON_H

#include "lulzjs.h"
#include <curses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>

void
__options (JSContext* cx, WINDOW* win, JSObject* options, JSBool apply)
{
    jsval jsAttrs; JS_GetProperty(cx, options, "at", &jsAttrs);
    if (!JSVAL_IS_INT(jsAttrs)) {
        JS_GetProperty(cx, options, "attribute", &jsAttrs);

        if (!JSVAL_IS_INT(jsAttrs)) {
            JS_GetProperty(cx, options, "attributes", &jsAttrs);
        }
    }

    if (JSVAL_IS_INT(jsAttrs)) {
        if (apply) {
            wattron(win, JSVAL_TO_INT(jsAttrs));
        }
        else {
            wattroff(win, JSVAL_TO_INT(jsAttrs));
        }
    }

    jsval jsForeground; JS_GetProperty(cx, options, "fg", &jsForeground);
    if (!JSVAL_IS_INT(jsForeground)) {
        JS_GetProperty(cx, options, "foreground", &jsForeground);
    }

    jsval jsBackground; JS_GetProperty(cx, options, "bg", &jsBackground);
    if (!JSVAL_IS_INT(jsBackground)) {
        JS_GetProperty(cx, options, "background", &jsBackground);
    }

    short fg = JSVAL_IS_INT(jsForeground) ? JSVAL_TO_INT(jsForeground) : -1;
    short bg = JSVAL_IS_INT(jsBackground) ? JSVAL_TO_INT(jsBackground) : -1;

    if (fg == -1 && bg == -1) {
        return;
    }

    char pair[3] = {0};
    sprintf(&pair[0], "%1d", (fg<0?0:fg)); // I've to give 0 if it's using the default
    sprintf(&pair[1], "%1d", (bg<0?0:bg)); // value because it fucks up with -1

    short c_pair = atoi(pair);
    if (apply) {
        init_pair(c_pair, fg, bg);
        wattron(win, COLOR_PAIR(c_pair));
    }
    else {
        wattroff(win, COLOR_PAIR(c_pair));
    }
}

#endif
