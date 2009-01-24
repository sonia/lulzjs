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

void
__updateACS (JSContext* cx)
{
    jsval property;

    JS_GetProperty(cx, JS_GetGlobalObject(cx), "ncurses", &property);
    JSObject* AlternativeChars   = JS_NewObject(cx, NULL, NULL, NULL);
    jsval     jsAlternativeChars = OBJECT_TO_JSVAL(AlternativeChars);
    JS_SetProperty(cx, JSVAL_TO_OBJECT(property), "AlternativeChars", &jsAlternativeChars);
        property = INT_TO_JSVAL(ACS_BLOCK);
        JS_SetProperty(cx, AlternativeChars, "Block", &property);
        property = INT_TO_JSVAL(ACS_BOARD);
        JS_SetProperty(cx, AlternativeChars, "Board", &property);
        property = INT_TO_JSVAL(ACS_BTEE);
        JS_SetProperty(cx, AlternativeChars, "BottomT", &property);
        property = INT_TO_JSVAL(ACS_BULLET);
        JS_SetProperty(cx, AlternativeChars, "Bullet", &property);
        property = INT_TO_JSVAL(ACS_CKBOARD);
        JS_SetProperty(cx, AlternativeChars, "CheckerBoard", &property);
        property = INT_TO_JSVAL(ACS_DARROW);
        JS_SetProperty(cx, AlternativeChars, "DownArrow", &property);
        property = INT_TO_JSVAL(ACS_DEGREE);
        JS_SetProperty(cx, AlternativeChars, "Degree", &property);
        property = INT_TO_JSVAL(ACS_DIAMOND);
        JS_SetProperty(cx, AlternativeChars, "Diamond", &property);
        property = INT_TO_JSVAL(ACS_GEQUAL);
        JS_SetProperty(cx, AlternativeChars, "GreaterEqual", &property);
        property = INT_TO_JSVAL(ACS_HLINE);
        JS_SetProperty(cx, AlternativeChars, "HorizontalLine", &property);
        property = INT_TO_JSVAL(ACS_LANTERN);
        JS_SetProperty(cx, AlternativeChars, "Lantern", &property);
        property = INT_TO_JSVAL(ACS_LARROW);
        JS_SetProperty(cx, AlternativeChars, "LeftArrow", &property);
        property = INT_TO_JSVAL(ACS_LEQUAL);
        JS_SetProperty(cx, AlternativeChars, "LessEqual", &property);
        property = INT_TO_JSVAL(ACS_LLCORNER);
        JS_SetProperty(cx, AlternativeChars, "LowerLeftCorner", &property);
        property = INT_TO_JSVAL(ACS_LRCORNER);
        JS_SetProperty(cx, AlternativeChars, "LowerRightCorner", &property);
        property = INT_TO_JSVAL(ACS_LTEE);
        JS_SetProperty(cx, AlternativeChars, "LeftT", &property);
        property = INT_TO_JSVAL(ACS_NEQUAL);
        JS_SetProperty(cx, AlternativeChars, "NotEqual", &property);
        property = INT_TO_JSVAL(ACS_PI);
        JS_SetProperty(cx, AlternativeChars, "GreekPi", &property);
        property = INT_TO_JSVAL(ACS_PLMINUS);
        JS_SetProperty(cx, AlternativeChars, "PlusMinus", &property);
        property = INT_TO_JSVAL(ACS_PLUS);
        JS_SetProperty(cx, AlternativeChars, "Plus", &property);
        property = INT_TO_JSVAL(ACS_RARROW);
        JS_SetProperty(cx, AlternativeChars, "RightArrow", &property);
        property = INT_TO_JSVAL(ACS_RTEE);
        JS_SetProperty(cx, AlternativeChars, "RightT", &property);
        property = INT_TO_JSVAL(ACS_S1);
        JS_SetProperty(cx, AlternativeChars, "Scan1", &property);
        property = INT_TO_JSVAL(ACS_S3);
        JS_SetProperty(cx, AlternativeChars, "Scan3", &property);
        property = INT_TO_JSVAL(ACS_S7);
        JS_SetProperty(cx, AlternativeChars, "Scan7", &property);
        property = INT_TO_JSVAL(ACS_S9);
        JS_SetProperty(cx, AlternativeChars, "Scan9", &property);
        property = INT_TO_JSVAL(ACS_STERLING);
        JS_SetProperty(cx, AlternativeChars, "Pound", &property);
        property = INT_TO_JSVAL(ACS_TTEE);
        JS_SetProperty(cx, AlternativeChars, "TopT", &property);
        property = INT_TO_JSVAL(ACS_UARROW);
        JS_SetProperty(cx, AlternativeChars, "UpperArrow", &property);
        property = INT_TO_JSVAL(ACS_ULCORNER);
        JS_SetProperty(cx, AlternativeChars, "UpperLeftCorner", &property);
        property = INT_TO_JSVAL(ACS_URCORNER);
        JS_SetProperty(cx, AlternativeChars, "UpperRightCorner", &property);
        property = INT_TO_JSVAL(ACS_VLINE);
        JS_SetProperty(cx, AlternativeChars, "VerticalLine", &property);
        property = INT_TO_JSVAL(ACS_STERLING);
        JS_SetProperty(cx, AlternativeChars, "Pound", &property);
        property = INT_TO_JSVAL(ACS_TTEE);
        JS_SetProperty(cx, AlternativeChars, "TopT", &property);
        property = INT_TO_JSVAL(ACS_UARROW);
        JS_SetProperty(cx, AlternativeChars, "UpperArrow", &property);
        property = INT_TO_JSVAL(ACS_ULCORNER);
        JS_SetProperty(cx, AlternativeChars, "UpperLeftCorner", &property);
        property = INT_TO_JSVAL(ACS_URCORNER);
        JS_SetProperty(cx, AlternativeChars, "UpperRightCorner", &property);
        property = INT_TO_JSVAL(ACS_VLINE);
        JS_SetProperty(cx, AlternativeChars, "VerticalLine", &property);
}

#endif
