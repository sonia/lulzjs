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

#include "lulzjs.h"

JSBool
js_ObjectIs (JSContext* cx, jsval check, const char* name)
{
    jsval jsObj; JS_GetProperty(cx, JS_GetGlobalObject(cx), "Object", &jsObj);
    JSObject* Obj = JSVAL_TO_OBJECT(jsObj);

    jsval newArgv[] = {
        check,
        STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, name), strlen(name)))
    };
    jsval ret; JS_CallFunctionName(cx, Obj, "is", 2, newArgv, &ret);

    return JSVAL_TO_BOOLEAN(ret);
}

jsint
js_parseInt (JSContext* cx, jsval number, int base)
{
    jsval ret;

    if (base >= 2 && base <= 36) {
        jsval argv[] = {number, INT_TO_JSVAL(base)};
        JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "parseInt", 2, argv, &ret);
    }
    else {
        jsval argv[] = {number};
        JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "parseInt", 1, argv, &ret);
    }

    if (strcmp(JS_GetStringBytes(JS_ValueToString(cx, ret)), "NaN") == 0) {
        return 0;
    }

    jsint nret; JS_ValueToInt32(cx, ret, &nret);
    return nret;
}

jsdouble
js_parseFloat (JSContext* cx, jsval number)
{
    jsval ret;

    jsval argv[] = {number};
    JS_CallFunctionName(cx, JS_GetGlobalObject(cx), "parseFloat", 1, argv, &ret);

    if (strcmp(JS_GetStringBytes(JS_ValueToString(cx, ret)), "NaN") == 0) {
        return 0;
    }
    
    jsdouble nret; JS_ValueToNumber(cx, ret, &nret);
    return nret;
}

