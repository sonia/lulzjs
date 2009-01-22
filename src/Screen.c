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

#include "Screen.h"

static JSContext* signalCx;
static JSObject*  signalObject;
void __Screen_resize (int signum) {
    jsval ret; JS_CallFunctionName(signalCx, signalObject, "onResize", 0, NULL, &ret);
}

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

    signalCx     = cx;
    signalObject = object;
    signal(SIGWINCH, __Screen_resize);

    initscr();

    if (has_colors()) {
        start_color();
        use_default_colors();
    }

    ScreenInformation* data = JS_malloc(cx, sizeof(ScreenInformation));
    JS_SetPrivate(cx, object, data);

    if (argc > 0) {
        JS_ValueToObject(cx, argv[0], &options);
    }
    else {
        options = JS_NewObject(cx, NULL, NULL, NULL);
    }
    
    if (argc > 1 && JS_OBJECT_IS(cx, argv[1], "Function")) {
        JS_SetProperty(cx, object, "onResize", &argv[1]);
    }
    else {
        jsval jsfunc;
        JS_GetProperty(cx, JS_GetGlobalObject(cx), "Function", &jsfunc);
        JS_GetProperty(cx, JSVAL_TO_OBJECT(jsfunc), "empty", &jsfunc);
        JS_SetProperty(cx, object, "onResize", &jsfunc);
    }

    jsval option;

    // Echoing of inputted keys
    JS_GetProperty(cx, options, "echo", &option);
    JSBool js_echo; JS_ValueToBoolean(cx, option, &js_echo);
    if (JSVAL_IS_VOID(option) || !js_echo) {
        noecho();
        data->echo = JS_FALSE;
    }
    else {
        echo();
        data->echo = JS_TRUE;
    }

    // Buffering type
    JS_GetProperty(cx, options, "buffering", &option);
    jsint js_buffering = JS_ParseInt(cx, option, 0);
    switch (js_buffering) {
        case Raw   : raw();    data->buffering = Raw; break;
        case CBreak: cbreak(); data->buffering = CBreak; break;
        default    : data->buffering = Normal; break;
    }

    // Keypad initialization
    JS_GetProperty(cx, options, "keypad", &option);
    JSBool js_keypad; JS_ValueToBoolean(cx, option, &js_keypad);
    if (JSVAL_IS_VOID(option) || js_keypad) {
        keypad(stdscr, TRUE);
        data->keypad = JS_TRUE;
    }
    else {
        data->keypad = JS_FALSE;
    }

    return JS_TRUE;
}

void
Screen_finalize (JSContext* cx, JSObject* object)
{
    ScreenInformation* data = JS_GetPrivate(cx, object);

    if (data) {
        switch (data->buffering) {
            case Raw   : noraw(); break;
            case CBreak: nocbreak(); break;
        }

        if (data->keypad) {
            keypad(stdscr, FALSE);
        }

        if (!data->echo) {
            echo();
        }

        endwin();

        JS_free(cx, data);
    }
}

JSBool
Screen_getChar (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    *rval = INT_TO_JSVAL(getch());
    return JS_TRUE;
}

JSBool
Screen_print (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    switch (argc) {
        case 1: {
        printw(JS_GetStringBytes(JS_ValueToString(cx, argv[0])));
        } break;

        case 2: {
        char* string = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
        JSObject* options; JS_ValueToObject(cx, argv[1], &options);


        } break;

        case 3: {
        char* string = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
        jsint x; JS_ValueToInt32(cx, argv[1], &x);
        jsint y; JS_ValueToInt32(cx, argv[2], &y);

        mvprintw(y, x, string);
        } break;

        case 4: {
        } break;
    }

    return JS_TRUE;
}

