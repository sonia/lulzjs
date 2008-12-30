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

#ifndef _CORE_H
#define _CORE_H

#include "jsapi.h"
#include "jsdbgapi.h"
#include "Misc.h"
#include <libgen.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static JSClass Core_class = {
    "Core", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

extern JSObject* Core_initialize (JSContext *cx);

extern JSBool Core_include (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval); 
extern JSBool Core_require (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

short __Core_include (JSContext* cx, const char* path);
char* __Core_getPath (JSContext* cx, const char* fileName);

static JSFunctionSpec Core_methods[] = {
    {"include", Core_include, 0, 0, 0},
    {"require", Core_require, 0, 0, 0},
    {NULL}
};

#endif
