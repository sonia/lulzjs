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
// Thanks to Sonia for this piece of OMG WTF IS THAT? :O

#ifndef _SYSTEM_CRYPT_SHA1_H
#define _SYSTEM_CRYPT_SHA1_H

#include "lulzjs.h"
#include <stdint.h>

extern JSBool exec (JSContext* cx);
extern JSBool SHA1_initialize (JSContext* cx);

extern JSBool SHA1_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern void   SHA1_finalize (JSContext* cx, JSObject* object); 

static JSClass SHA1_class = {
    "SHA1", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, SHA1_finalize
};

#include "private.h"

void __SHA1_transform (uint32_t state[5], uint8_t buffer[64]);
void __SHA1_init (SHA1_ctx *ctx);
void __SHA1_update (SHA1_ctx *ctx, uint8_t *data, const unsigned int len);
void __SHA1_final (SHA1_ctx *ctx);

extern JSBool SHA1_toString (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
void __SHA1_toString (SHA1_ctx *ctx, char out[41]);

static JSFunctionSpec SHA1_methods[] = {
    {"toString", SHA1_toString, 0, 0, 0},
    {NULL}
};

static JSFunctionSpec SHA1_static_methods[] = {
    {NULL}
};

#endif
