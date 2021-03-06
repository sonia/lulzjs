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

#ifndef _SYSTEM_IO_STREAM_H
#define _SYSTEM_IO_STREAM_H

#include "lulzjs.h"

extern JSBool exec (JSContext* cx);
extern JSBool Stream_initialize (JSContext* cx);

static JSClass Stream_class = {
    "Stream", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

#include "private.h"

extern JSBool Stream_read (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Stream_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Stream_methods[] = {
    {"read",   Stream_read,  0, 0, 0},
    {"write",  Stream_write, 0, 0, 0},
    {NULL}
};

#endif
