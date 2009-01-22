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

#include "jsapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

extern JSBool js_ObjectIs (JSContext* cx, jsval check, const char* name);
#define JS_OBJECT_IS(cx, check, name) js_ObjectIs(cx, check, name)

extern jsint js_parseInt (JSContext* cx, jsval number, int base);
#define JS_ParseInt(cx, number, base) js_parseInt(cx, number, base)

extern jsdouble js_parseFloat (JSContext* cx, jsval number);
#define JS_ParseFloat(cx, number, base) js_parseInt(cx, number)

