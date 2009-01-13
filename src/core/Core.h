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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Not cross platform
#include <libgen.h>
#include <dlfcn.h>
#include <unistd.h>

#include "Misc.h"
#include "Hash.h"

static char** included       = NULL;
static size_t includedNumber = 0;

Hash* timeouts;
Hash* intervals;

static JSClass Core_class = {
    "Core", JSCLASS_GLOBAL_FLAGS|JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

extern JSObject* Core_initialize (JSContext *cx, const char* script);

extern JSBool Core_include (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval); 
extern JSBool Core_require (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
extern JSBool Core_GC (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern JSBool Core_die (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
extern JSBool Core_exit (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern JSBool Core_ENV (JSContext* cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

/*
 * Execute a system command and return the output.
 *
 * PARAMS:
 *      command (String) > The string that the system will execute.
 *
 * RETURN:
 *     String < The command's output.
 */
extern JSBool Core_exec (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

extern JSBool Core_sleep (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

const char* __Core_getScriptName (JSContext* cx);
char*       __Core_getRootPath (JSContext* cx, const char* fileName);
char*       __Core_getPath (JSContext* cx, const char* fileName);
JSBool      __Core_include (JSContext* cx, const char* path);
JSBool      __Core_isIncluded (const char* path);

static JSFunctionSpec Core_methods[] = {
    {"include", Core_include, 0, 0, 0},
    {"require", Core_require, 0, 0, 0},
    {"GC",      Core_GC,      0, 0, 0},

    {"die",  Core_die,  0, 0, 0},
    {"exit", Core_exit, 0, 0, 0},

    {"ENV",   Core_ENV,  0, 0, 0},
    {"exec",  Core_exec, 0, 0, 0},
    {"sleep", Core_sleep, 0, 0, 0},
    {NULL}
};

#endif
