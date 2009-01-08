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

#include "Socket.h"

short exec (JSContext* cx) { return Socket_initialize(cx); }

short
Socket_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(jsParent), "Net", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Socket_class,
        Socket_constructor, 0, NULL, Socket_methods, NULL, Socket_static_methods
    );

    if (!object)
        return 0;

    // Default properties
    jsval property;

    // Address families.
    property = INT_TO_JSVAL(AF_INET);
    JS_SetProperty(cx, object, "AF_INET", &property);

    // Socket types.
    property = INT_TO_JSVAL(SOCK_STREAM);
    JS_SetProperty(cx, object, "SOCK_STREAM", &property);
    property = INT_TO_JSVAL(SOCK_DGRAM);
    JS_SetProperty(cx, object, "SOCK_DGRAM", &property);

    // Protocol type.
    property = INT_TO_JSVAL(PF_UNSPEC);
    JS_SetProperty(cx, object, "PF_UNSPEC", &property);

    return 1;
}

JSBool
Socket_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    unsigned family   = AF_INET;
    unsigned type     = SOCK_STREAM;
    unsigned protocol = PF_UNSPEC;

    if (argc > 3) {
        JS_ReportError(cx, "Too many arguments.");
        return JS_FALSE;
    }

    switch (argc) {
        case 3: protocol = JSVAL_TO_INT(argv[2]);
        case 2: type     = JSVAL_TO_INT(argv[1]);
        case 1: family   = JSVAL_TO_INT(argv[0]);
    }

    SocketInformation* data = JS_malloc(cx, sizeof(SocketInformation));
    JS_SetPrivate(cx, object, data);

    data->socket   = socket(family, type, protocol);
    data->family   = family;
    data->type     = type;
    data->protocol = protocol;

    return JS_TRUE;
}

void
Socket_finalize (JSContext* cx, JSObject* object)
{
    SocketInformation* data = JS_GetPrivate(cx, object);

    if (data) {
        close(data->socket);
        JS_free(cx, data);
    }
}

JSBool
Socket_connect (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* host;
    int port;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "si", &host)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    struct sockaddr_in addrin;

    addrin.sin_family      = data->family;
    addrin.sin_port        = htons((u_short)port);

    if (__Socket_isIPv4(cx, host)) {
        addrin.sin_addr.s_addr = inet_addr(host);
    }
    else {
        struct hostent* gethost;

        gethost = gethostbyname(host);

        if (!gethost) {
            *rval = JSVAL_FALSE;
            return JS_TRUE;
        }

        addrin.sin_addr.s_addr = inet_addr(gethost->h_name);
    }

    if (connect(data->socket, (struct sockaddr*) &addrin, sizeof(sockaddr)) < 0) {
        *rval = JSVAL_FALSE;
    }
    else {
        *rval = JSVAL_TRUE;
    }

    return JS_TRUE;
}

JSBool
Socket_send (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    return JS_TRUE;
}

JSBool
Socket_receive (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned size;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    return JS_TRUE;
}

JSBool
__Socket_isIPv4 (JSContext* cx, const char* host)
{
    char* ip     = "^(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)$";
    JSObject* re = JS_NewRegExpObject(cx, ip, strlen(ip), 0);

    jsval jsMatches;
    JS_ExecuteRegExp(cx, re, JS_NewString(cx, JS_strdup(cx, host), strlen(host)), JS_FALSE, &jsMatches);

    if (JSVAL_IS_NULL(jsMatches)) {
        return JS_FALSE;
    }

    JSObject* matches = JSVAL_TO_OBJECT(jsMatches);

    jsuint length;
    JS_GetArrayLength(cx, matches, &length);

    jsuint i;
    for (i = 0; i < length; i++) {
        jsval jsClass;
        JS_GetElement(cx, matches, i, &jsClass);

        int class = JSVAL_TO_INT(jsClass);
        if (class < 0 || class > 255) {
            return JS_FALSE;
        }
    }

    return JS_TRUE;
}
