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

        return JS_TRUE;
    }

    return JS_FALSE;
}

