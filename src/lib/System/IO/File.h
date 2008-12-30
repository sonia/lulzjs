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

#ifndef _SYSTEM_IO_FILE_H
#define _SYSTEM_IO_FILE_H

#include "jsapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern short exec (JSContext* context);
extern short File_initialize (JSContext* context);
extern void  File_finalize (JSContext* context, JSObject* object); 

static JSClass File_class = {
    "File", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, File_finalize
};

#include "File_private.h"

extern JSBool File_constructor (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool File_read (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool File_write (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool File_isEnd (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool File_static_exists (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval);

static JSFunctionSpec File_methods[] = {
    {"read",   File_read,   0, 0, 0},
    {"write",  File_write,  0, 0, 0},
    {"isEnd",  File_isEnd,  0, 0, 0},
    {NULL}
};

static JSFunctionSpec File_static_methods[] = {
    {"exists", File_static_exists, 0, 0, 0},
    {NULL}
};

#endif
