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

#include "Window.h"

JSBool exec (JSContext* cx) { return Window_initialize(cx); }

JSBool
Window_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "ncurses", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Window_class,
        Window_constructor, 1, NULL, Window_methods, NULL, NULL
    );

    if (object) {
        // Default properties
        jsval property;

        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Window_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* parent;
    JSObject* options;
    jsval x, y, width, height;
    jsint offset = 0;

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    if (argc == 2) {
        JS_ValueToObject(cx, argv[0], &parent);

        if (!JS_OBJECT_IS(cx, OBJECT_TO_JSVAL(parent), "ncurses.Window")) {
            JS_ReportError(cx, "You have to pass a Window object.");
            return JS_FALSE;
        }

        offset = 1;
    }

    jsval val;
    JS_ValueToObject(cx, argv[offset], &options);
    JS_GetProperty(cx, options, "width", &width);
    JS_GetProperty(cx, options, "height", &height);
    JS_GetProperty(cx, options, "x", &x);
    JS_GetProperty(cx, options, "y", &y);

    if (!JSVAL_IS_INT(width) || !JSVAL_IS_INT(height) || !JSVAL_IS_INT(x) || !JSVAL_IS_INT(y)) {
        JS_ReportError(cx, "An option is missing or isn't an int.");
        return JS_FALSE;
    }
    
    WINDOW* win = JS_malloc(cx, sizeof(WINDOW));
    if (argc == 2) {
        WINDOW* parentWin = JS_GetPrivate(cx, parent);
        win = subwin(parentWin,
            JSVAL_TO_INT(height), JSVAL_TO_INT(width),
            JSVAL_TO_INT(y), JSVAL_TO_INT(x)
        );
    }
    else {
        win = newwin(
            JSVAL_TO_INT(height), JSVAL_TO_INT(width),
            JSVAL_TO_INT(y), JSVAL_TO_INT(x)
        );
    }
    JS_SetPrivate(cx, object, win);

    JSObject* Size   = JS_NewObject(cx, NULL, NULL, NULL);
    jsval     jsSize = OBJECT_TO_JSVAL(Size);
    JS_SetProperty(cx, object, "Size", &jsSize);
    __Window_updateSize(cx, object);

    JSObject* Position   = JS_NewObject(cx, NULL, NULL, NULL);
    jsval     jsPosition = OBJECT_TO_JSVAL(Position);
    JS_SetProperty(cx, object, "Position", &jsPosition);
    __Window_updatePosition(cx, object);

    jsval jsBorder; JS_GetProperty(cx, options, "border", &jsBorder);
    JSBool border; JS_ValueToBoolean(cx, jsBorder, &border);
    if (border) {
        box(win, 0, 0);
    }

    wrefresh(win);

    return JS_TRUE;
}

void
Window_finalize (JSContext* cx, JSObject* object)
{
    WINDOW* win = JS_GetPrivate(cx, object);

    if (win) {
        JS_free(cx, win);
    }
}

JSBool
Window_refresh (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    WINDOW* win = JS_GetPrivate(cx, object);

    wrefresh(win);
    return JS_TRUE;
}

JSBool
Window_redraw (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc != 2 && argc != 0) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    WINDOW* win = JS_GetPrivate(cx, object);

    switch (argc) {
        case 0: {
            wrefresh(win);
        } break;

        case 2: {
            jsint beg; JS_ValueToInt32(cx, argv[0], &beg);
            jsint num; JS_ValueToInt32(cx, argv[1], &num);

            wredrawln(win, beg, num);
        } break;
    }

    return JS_TRUE;

}

JSBool
Window_printChar (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    WINDOW* win = JS_GetPrivate(cx, object);

    jsint ch; JS_ValueToInt32(cx, argv[0], &ch);

    if (argc == 1){
        waddch(win, ch);
    }
    else if (argc == 2) {
        JSObject* options; JS_ValueToObject(cx, argv[1], &options);

        jsval x, y;
        JS_GetProperty(cx, options, "x", &x);
        JS_GetProperty(cx, options, "y", &y);

        jsval jsEcho; JS_GetProperty(cx, options, "echo", &jsEcho);
        JSBool echo; JS_ValueToBoolean(cx, jsEcho, &echo);

        __options(cx, win, options, JS_TRUE);
        if (echo) {
            wechochar(win, ch);
        }
        else if (!JSVAL_IS_INT(x) && !JSVAL_IS_INT(y)) {
            waddch(win, ch);
        }
        else {
            mvwaddch(win,
                JSVAL_IS_INT(y) ? JSVAL_TO_INT(y) : 0,
                JSVAL_IS_INT(x) ? JSVAL_TO_INT(x) : 0,
                ch
            );
        }
        __options(cx, win, options, JS_FALSE);
    }

    return JS_TRUE;
}

JSBool
Window_getChar (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* options;

    WINDOW* win = JS_GetPrivate(cx, object);

    if (argc == 0) {
        *rval = INT_TO_JSVAL(wgetch(win));
    }
    else {
        JS_ValueToObject(cx, argv[0], &options);

        if (!options) {
            JS_ReportError(cx, "Options isn't a valid object.");
            return JS_FALSE;
        }

        jsval x, y;
        JS_GetProperty(cx, options, "x", &x);
        JS_GetProperty(cx, options, "y", &y);

        if (!JSVAL_IS_INT(x) || !JSVAL_IS_INT(y)) {
            JS_ReportError(cx, "An option is missing or isn't an int.");
            return JS_FALSE;
        }

        *rval = INT_TO_JSVAL(mvwgetch(win, JSVAL_TO_INT(y), JSVAL_TO_INT(x)));
    }

    return JS_TRUE;
}

JSBool
Window_printString (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    WINDOW* win = JS_GetPrivate(cx, object);

    if (argc == 1){
        wprintw(win, JS_GetStringBytes(JS_ValueToString(cx, argv[0])));
    }
    else if (argc == 2) {
        JSObject* options; JS_ValueToObject(cx, argv[1], &options);

        jsval x, y;
        JS_GetProperty(cx, options, "x", &x);
        JS_GetProperty(cx, options, "y", &y);

        __options(cx, win, options, JS_TRUE);
        if (!JSVAL_IS_INT(x) && !JSVAL_IS_INT(y)) {
            wprintw(win, JS_GetStringBytes(JS_ValueToString(cx, argv[0])));
        }
        else {
            mvwprintw(win,
                JSVAL_IS_INT(y) ? JSVAL_TO_INT(y) : 0,
                JSVAL_IS_INT(x) ? JSVAL_TO_INT(x) : 0,
                JS_GetStringBytes(JS_ValueToString(cx, argv[0]))
            );
        }
        __options(cx, win, options, JS_FALSE);
    }

    return JS_TRUE;
}

void
__Window_updateSize (JSContext* cx, JSObject* object)
{
    int height, width;
    WINDOW* win = JS_GetPrivate(cx, object);
    getmaxyx(win, height, width);

    jsval jsSize; JS_GetProperty(cx, object, "Size", &jsSize);
    JSObject* Size = JSVAL_TO_OBJECT(jsSize);

    jsval property;
    property = INT_TO_JSVAL(height);
    JS_SetProperty(cx, Size, "Height", &property);
    property = INT_TO_JSVAL(width);
    JS_SetProperty(cx, Size, "Width", &property);
}

void
__Window_updatePosition (JSContext* cx, JSObject* object)
{
    int y, x;
    WINDOW* win = JS_GetPrivate(cx, object);
    getbegyx(win, y, x);

    jsval jsPosition; JS_GetProperty(cx, object, "Position", &jsPosition);
    JSObject* Position = JSVAL_TO_OBJECT(jsPosition);

    jsval property;
    property = INT_TO_JSVAL(y);
    JS_SetProperty(cx, Position, "Y", &property);
    property = INT_TO_JSVAL(x);
    JS_SetProperty(cx, Position, "X", &property);
}

