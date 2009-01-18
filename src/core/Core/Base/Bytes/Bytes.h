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

#ifndef _SYSTEM_THREAD_H
#define _SYSTEM_THREAD_H

#include "jsapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern JSBool exec (JSContext* cx);
extern JSBool Bytes_initialize (JSContext* cx);

extern JSBool Bytes_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern void   Bytes_finalize (JSContext* cx, JSObject* object); 

static JSClass Bytes_class = {
    "Bytes", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Bytes_finalize
};

extern JSBool Bytes_byteAt (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Bytes_toArray (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec Bytes_methods[] = {
    {NULL}
};

static JSFunctionSpec Bytes_static_methods[] = {
    {NULL}
};

#endif
