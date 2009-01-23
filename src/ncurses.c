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

#include "ncurses.h"

JSBool exec (JSContext* cx) { return ncurses_initialize(cx); }

JSBool
ncurses_initialize (JSContext* cx)
{
    JSObject* object = JS_DefineObject(
        cx, JS_GetGlobalObject(cx),
        ncurses_class.name, &ncurses_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );

    if (object) {
        JS_DefineFunctions(cx, object, ncurses_methods);

        jsval property;

        JSObject* Buffering   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsBuffering = OBJECT_TO_JSVAL(Buffering);
        JS_SetProperty(cx, object, "Buffering", &jsBuffering);
            property = INT_TO_JSVAL(Normal);
            JS_SetProperty(cx, Buffering, "Normal", &property);
            property = INT_TO_JSVAL(Raw);
            JS_SetProperty(cx, Buffering, "Raw", &property);
            property = INT_TO_JSVAL(CBreak);
            JS_SetProperty(cx, Buffering, "CBreak", &property);

        JSObject* Colors   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsColors = OBJECT_TO_JSVAL(Colors);
        JS_SetProperty(cx, object, "Colors", &jsColors);
            property = INT_TO_JSVAL(-1);
            JS_SetProperty(cx, Colors, "Normal", &property);
            property = INT_TO_JSVAL(COLOR_BLACK);
            JS_SetProperty(cx, Colors, "Black", &property);
            property = INT_TO_JSVAL(COLOR_RED);
            JS_SetProperty(cx, Colors, "Red", &property);
            property = INT_TO_JSVAL(COLOR_GREEN);
            JS_SetProperty(cx, Colors, "Green", &property);
            property = INT_TO_JSVAL(COLOR_YELLOW);
            JS_SetProperty(cx, Colors, "Yellow", &property);
            property = INT_TO_JSVAL(COLOR_BLUE);
            JS_SetProperty(cx, Colors, "Blue", &property);
            property = INT_TO_JSVAL(COLOR_MAGENTA);
            JS_SetProperty(cx, Colors, "Magenta", &property);
            property = INT_TO_JSVAL(COLOR_CYAN);
            JS_SetProperty(cx, Colors, "Cyan", &property);
            property = INT_TO_JSVAL(COLOR_WHITE);
            JS_SetProperty(cx, Colors, "White", &property);

        JSObject* Attributes   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsAttributes = OBJECT_TO_JSVAL(Attributes);
        JS_SetProperty(cx, object, "Attributes", &jsAttributes);
            property = INT_TO_JSVAL(A_NORMAL); // Normal display (no highlight)
            JS_SetProperty(cx, Attributes, "Normal", &property);
            property = INT_TO_JSVAL(A_BLINK); // Blinking
            JS_SetProperty(cx, Attributes, "Blink", &property);
            property = INT_TO_JSVAL(A_BOLD); // Extra bright or bold
            JS_SetProperty(cx, Attributes, "Bold", &property);
            property = INT_TO_JSVAL(A_DIM); // Half bright
            JS_SetProperty(cx, Attributes, "Dim", &property);
            property = INT_TO_JSVAL(A_REVERSE); // Reverse video
            JS_SetProperty(cx, Attributes, "Reverse", &property);
            property = INT_TO_JSVAL(A_STANDOUT); // Best highlighting mode of the terminal.
            JS_SetProperty(cx, Attributes, "Standout", &property);
            property = INT_TO_JSVAL(A_UNDERLINE); // Underlining
            JS_SetProperty(cx, Attributes, "Underline", &property);
            property = INT_TO_JSVAL(A_PROTECT); // Protected mode
            JS_SetProperty(cx, Attributes, "Protect", &property);
            property = INT_TO_JSVAL(A_INVIS);
            JS_SetProperty(cx, Attributes, "Invisible", &property);

        JSObject* Cursor   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsCursor = OBJECT_TO_JSVAL(Cursor);
        JS_SetProperty(cx, object, "Cursor", &jsCursor);
            property = INT_TO_JSVAL(0);
            JS_SetProperty(cx, Cursor, "Invisible", &property);
            property = INT_TO_JSVAL(1);
            JS_SetProperty(cx, Cursor, "Normal", &property);
            property = INT_TO_JSVAL(2);
            JS_SetProperty(cx, Cursor, "Visible", &property);

        JSObject* Keys   = JS_NewObject(cx, NULL, NULL, NULL);
        jsval     jsKeys = OBJECT_TO_JSVAL(Keys);
        JS_SetProperty(cx, object, "Keys", &jsKeys);
            property = INT_TO_JSVAL(KEY_BREAK);
            JS_SetProperty(cx, Keys, "Break", &property);
            property = INT_TO_JSVAL(KEY_DOWN);
            JS_SetProperty(cx, Keys, "Down", &property);
            property = INT_TO_JSVAL(KEY_UP);
            JS_SetProperty(cx, Keys, "Up", &property);
            property = INT_TO_JSVAL(KEY_LEFT);
            JS_SetProperty(cx, Keys, "Left", &property);
            property = INT_TO_JSVAL(KEY_RIGHT);
            JS_SetProperty(cx, Keys, "Right", &property);
            property = INT_TO_JSVAL(KEY_HOME);
            JS_SetProperty(cx, Keys, "Home", &property);
            property = INT_TO_JSVAL(KEY_BACKSPACE);
            JS_SetProperty(cx, Keys, "Backspace", &property);
            property = OBJECT_TO_JSVAL(JS_GetFunctionObject(
                JS_NewFunction(cx, ncurses_KEY_F, 1, 0, NULL, "KEY_F")
            ));
            JS_SetProperty(cx, Keys, "F", &property);
            property = INT_TO_JSVAL(KEY_DL);
            JS_SetProperty(cx, Keys, "DL", &property);
            property = INT_TO_JSVAL(KEY_IL);
            JS_SetProperty(cx, Keys, "IL", &property);
            property = INT_TO_JSVAL(KEY_DC);
            JS_SetProperty(cx, Keys, "DC", &property);
            property = INT_TO_JSVAL(KEY_IC);
            JS_SetProperty(cx, Keys, "IC", &property);
            property = INT_TO_JSVAL(KEY_EIC);
            JS_SetProperty(cx, Keys, "EIC", &property);
            property = INT_TO_JSVAL(KEY_CLEAR);
            JS_SetProperty(cx, Keys, "Clear", &property);
            property = INT_TO_JSVAL(KEY_EOS);
            JS_SetProperty(cx, Keys, "EOS", &property);

        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
ncurses_KEY_F (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    jsint n; JS_ValueToInt32(cx, argv[0], &n);
    *rval = INT_TO_JSVAL(KEY_F(n));

    return JS_TRUE;
}

