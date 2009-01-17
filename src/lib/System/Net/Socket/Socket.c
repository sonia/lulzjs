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

JSBool exec (JSContext* cx) { return Socket_initialize(cx); }

JSBool
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

    if (object) {
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

        // Other
        property = JSVAL_NULL;
        JS_SetProperty(cx, object, "INADDR_ANY", &property);

        return JS_TRUE;
    }

    return JS_FALSE;
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
        case 3: JS_ValueToInt32(cx, argv[2], &protocol);
        case 2: JS_ValueToInt32(cx, argv[1], &type);
        case 1: JS_ValueToInt32(cx, argv[0], &family);
    }

    SocketInformation* data = JS_malloc(cx, sizeof(SocketInformation));
    JS_SetPrivate(cx, object, data);

    data->socket    = socket(family, type, protocol);
    data->family    = family;
    data->type      = type;
    data->protocol  = protocol;
    data->connected = JS_FALSE;

    return JS_TRUE;
}

void
Socket_finalize (JSContext* cx, JSObject* object)
{
    SocketInformation* data = JS_GetPrivate(cx, object);

    if (data) {
        if (data->addr) {
            JS_free(cx, data->addr);
        }

        close(data->socket);
        JS_free(cx, data);
    }
}

JSBool
Socket_connect (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* host;
    int port;

    if (argc < 2 || !JS_ConvertArguments(cx, argc, argv, "si", &host, &port)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    struct sockaddr_in* addrin = JS_malloc(cx, sizeof(struct sockaddr_in));

    addrin->sin_family = data->family;
    addrin->sin_port   = htons((u_short) port);

    if (__Socket_isIPv4(host)) {
        addrin->sin_addr.s_addr = inet_addr(host);
    }
    else {
        const char* ip = __Socket_getHostByName(cx, host);

        if (!ip) {
            *rval = JSVAL_FALSE;
            return JS_TRUE;
        }

        addrin->sin_addr.s_addr = inet_addr(ip);
    }

    if (connect(data->socket, (struct sockaddr*) addrin, sizeof(struct sockaddr_in)) < 0) {
        data->connected = JS_FALSE;
    }
    else {
        data->connected = JS_TRUE;
    }

    data->addr = (struct sockaddr*) addrin;

    *rval = BOOLEAN_TO_JSVAL(data->connected);

    return JS_TRUE;
}

JSBool
Socket_listen (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    int port;
    int maxconn = 255;

    if (argc != 2) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    switch (argc) {
        case 3: JS_ValueToInt32(cx, argv[2], &maxconn); break;
        case 2: JS_ValueToInt32(cx, argv[1], &port); break;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    struct sockaddr_in* addrin = JS_malloc(cx, sizeof(struct sockaddr_in));

    addrin->sin_family = data->family;
    addrin->sin_port   = htons((u_short) port);

    if (JSVAL_IS_NULL(argv[0])) {
        addrin->sin_addr.s_addr = INADDR_ANY;
    }
    else {
        const char* ip = __Socket_getHostByName(cx, JS_GetStringBytes(JS_ValueToString(cx, argv[0])));
        
        if (!ip) {
            JS_ReportError(cx, "Couldn't resolve the hostname.");
            return JS_FALSE;
        }

        addrin->sin_addr.s_addr = inet_addr(ip);
    }

    if (bind(data->socket, (struct sockaddr*) addrin, sizeof(struct sockaddr_in)) < 0) {
        JS_ReportError(cx, "Bind failed, probably the port is already in use.");
        return JS_FALSE;
    }

    if (listen(data->socket, maxconn) < 0) {
        JS_ReportError(cx, "Listen failed.");
        return JS_FALSE;
    }

    data->addr = (struct sockaddr*) addrin;

    return JS_TRUE;
}

JSBool
Socket_accept (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    SocketInformation* data = JS_GetPrivate(cx, object);

    JSObject* sock = JS_NewObject(cx, &Socket_class, JS_GetPrototype(cx, object), NULL);

    int size = sizeof(struct sockaddr);

    SocketInformation* newData = JS_malloc(cx, sizeof(SocketInformation));
    newData->addr     = JS_malloc(cx, sizeof(struct sockaddr*));
    newData->socket   = accept(data->socket, newData->addr, &size);
    newData->family   = ((struct sockaddr_in*)newData->addr)->sin_family;
    JS_SetPrivate(cx, sock, newData);

    *rval = OBJECT_TO_JSVAL(sock);
    return JS_TRUE;
}

JSBool
Socket_send (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;
    unsigned flags = 0;

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    switch (argc) {
        case 2: JS_ValueToInt32(cx, argv[1], &flags);
        case 1: string = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    if (!data->connected) {
        JS_ReportError(cx, "The socket isn't connected.");
        return JS_FALSE;
    }
    
    *rval = INT_TO_JSVAL(send(data->socket, string, strlen(string), flags));

    return JS_TRUE;
}

JSBool
Socket_receive (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    unsigned size;
    unsigned flags = 0;

    if (argc < 1) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    switch (argc) {
        case 2: JS_ValueToInt32(cx, argv[1], &flags);
        case 1: JS_ValueToInt32(cx, argv[0], &size);
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    if (!data->connected) {
        JS_ReportError(cx, "The socket isn't connected.");
        return JS_FALSE;
    }

    char* string = JS_malloc(cx, size*sizeof(char)+1);

    unsigned offset = 0;

    while (offset != size) {
        offset += recv(data->socket, (string+offset), size, flags);
    }
    string[size] = '\0';

    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, strlen(string)));

    return JS_TRUE;
}

JSBool
Socket_static_getHostByName (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* host;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &host)) {
        JS_ReportError(cx, "Not enough paramters.");
        return JS_FALSE;
    }

    char* rHost = (char*) __Socket_getHostByName(cx, host);

    if (!rHost) {
        JS_ReportError(cx, "An error occurred while resolving the hostname.");
        return JS_FALSE;
    }

    *rval = STRING_TO_JSVAL(JS_NewString(cx, rHost, strlen(rHost)));
    return JS_TRUE;
}

const char*
__Socket_getHostByName (JSContext* cx, const char* host)
{
    struct hostent* hp = gethostbyname(host);
    struct in_addr in;

    if (!hp) {
        return NULL;
    }

    char* ip = JS_malloc(cx, INET6_ADDRSTRLEN*sizeof(char));
    int offset = 0;

    offset += sprintf(&ip[offset], "%u.", (unsigned char) hp->h_addr[0]);
    offset += sprintf((ip+offset), "%u.", (unsigned char) hp->h_addr[1]);
    offset += sprintf((ip+offset), "%u.", (unsigned char) hp->h_addr[2]);
    offset += sprintf((ip+offset), "%u",  (unsigned char) hp->h_addr[3]);

    return ip;
}

JSBool
Socket_static_isIPv4 (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* host;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &host)) {
        JS_ReportError(cx, "Not enough paramters.");
        return JS_FALSE;
    }

    *rval = BOOLEAN_TO_JSVAL(__Socket_isIPv4(host));
    return JS_TRUE;
}

JSBool
__Socket_isIPv4 (const char* host)
{
    char class[4];
    memset(class, 0, 4);

    short number;
    size_t i;
    for (i = 0, number = 0; i < strlen(host); i++) {
        if (number == 3) {
            return JS_FALSE;
        }

        if (host[i] == '.') {
            number = 0;
            
            short part = atoi(class);

            if (part < 0 || part > 255) {
                return JS_FALSE;
            }

            memset(class, 0, 4);
        }

        class[number] = host[i];
        number++;
    }

    return JS_TRUE;
}
