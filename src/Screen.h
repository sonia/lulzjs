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
#include "Window_private.h"
#include <signal.h>

void __Screen_resize (int signum);

extern JSBool exec (JSContext* cx);
extern JSBool Screen_initialize (JSContext* cx);

extern JSBool Screen_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern void   Screen_finalize (JSContext* cx, JSObject* object); 

static JSClass Screen_class = {
    "Screen", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Screen_finalize
};

#include "Screen_private.h"

extern JSBool Screen_cursorMode (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

void __Screen_updateSize (JSContext* cx, JSObject* object);
void __Screen_updateACS (JSContext* cx);

static JSFunctionSpec Screen_methods[] = {
    {"cursorMode", Screen_cursorMode, 0, 0, 0},
    {NULL}
};

#endif
