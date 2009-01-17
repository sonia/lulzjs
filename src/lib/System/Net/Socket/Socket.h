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

#ifndef _SYSTEM_NET_SOCKET_H
#define _SYSTEM_NET_SOCKET_H

#include "jsapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

extern JSBool exec (JSContext* cx);
extern JSBool Socket_initialize (JSContext* cx);

extern JSBool Socket_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern void  Socket_finalize (JSContext* cx, JSObject* object); 

static JSClass Socket_class = {
    "Socket", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, Socket_finalize
};

#include "private.h"

extern JSBool Socket_connect (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool Socket_listen (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Socket_accept (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool Socket_send (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
extern JSBool Socket_receive (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);

extern JSBool Socket_static_getHostByName (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
const char* __Socket_getHostByName (JSContext* cx, const char* host);

extern JSBool Socket_static_isIPv4 (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval);
JSBool __Socket_isIPv4 (const char* host);

static JSFunctionSpec Socket_methods[] = {
    {"connect", Socket_connect, 0, 0, 0},

    {"listen", Socket_listen, 0, 0, 0},
    {"accept", Socket_accept, 0, 0, 0},

    {"send",    Socket_send,    0, 0, 0},
    {"receive", Socket_receive, 0, 0, 0},

    {NULL}
};

static JSFunctionSpec Socket_static_methods[] = {
    {"getHostByName", Socket_static_getHostByName, 0, 0, 0},
    {"isIPv4",        Socket_static_isIPv4,        0, 0, 0},

    {NULL}
};

#endif
