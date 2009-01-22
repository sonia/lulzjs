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

        return JS_TRUE;
    }

    return JS_FALSE;
}

