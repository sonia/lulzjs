/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh.                                                             *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be useful.                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

#include "Screen.h"

JSBool exec (JSContext* cx) { return Screen_initialize(cx); }

JSBool
Screen_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "ncurses", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Screen_class,
        Screen_constructor, 1, NULL, Screen_methods, NULL, Screen_static_methods
    );

    if (object) {
        // Default properties
        jsval property;

        property = INT_TO_JSVAL(EOF);
        JS_SetProperty(cx, parent, "EOF", &property);

        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Screen_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* options;

    if (stdscr) {
        JS_ReportError(cx, "You can have only one Screen per program.");
        return JS_FALSE;
    }

    initscr();

    if (argc == 1) {
        JS_ValueToObject(cx, argv[0], &options);
    }
    else {
        options = JS_NewObject(cx, NULL, NULL, NULL);
    }

    jsval option;

    // Echoing of inputted keys
    JS_GetProperty(cx, options, "echo", &option);
    JSBool js_echo; JS_ValueToBoolean(cx, option, &js_echo);
    if (JSVAL_IS_VOID(option) || !js_echo) {
        noecho();
    }
    else {
        echo();
    }

    // Buffering type
    JS_GetProperty(cx, options, "buffering", &option);
    jsuint js_buffering; JS_ValueToInt32(cx, option, &js_buffering);
    switch (js_buffering) {
        case RAW   : raw(); break;
        case CBREAK: cbrea(); break;
    }

    // Keypad initialization
    JS_GetProperty(cx, options, "keypad", &option);
    JSBool js_keypad; JS_ValueToBoolean(cx, option, &js_keypad);
    if (JSVAL_IS_VOID(option) || js_keypad) {
        keypad(stdscr, TRUE);
    }

    return JS_TRUE;
}

void
Screen_finalize (JSContext* cx, JSObject* object)
{
    echo();
    endwin();
}
