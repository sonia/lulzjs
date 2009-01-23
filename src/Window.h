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

#ifndef _LULZJS_NCURSES_SCREEN_H
#define _LULZJS_NCURSES_SCREEN_H

#include "common.h"

extern JSBool exec (JSContext* cx);
extern JSBool Window_initialize (JSContext* cx);

extern JSBool Window_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern void   Window_finalize (JSContext* cx, JSObject* object); 

static JSClass Window_class = {
    "Window", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Window_finalize
};

extern JSBool Window_refresh (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Window_redraw (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool Window_getChar (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Window_printChar (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool Window_printString (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

void __Window_updateSize (JSContext* cx, JSObject* object);
void __Window_updatePosition (JSContext* cx, JSObject* object);

static JSFunctionSpec Window_methods[] = {
    {"refresh", Window_refresh, 0, 0, 0},
    {"redraw",  Window_redraw,  0, 0, 0},

    {"printChar", Window_printChar, 0, 0, 0},
    {"getChar",   Window_getChar,   0, 0, 0},

    {"printString", Window_printString, 0, 0, 0},

    {NULL}
};

#endif
