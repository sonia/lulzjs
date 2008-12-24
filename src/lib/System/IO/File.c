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

#include "File.h"

short exec (JSContext* context) { return File_initialize(context); }

short
File_initialize (JSContext* context)
{
    jsval jsParent;
    JS_GetProperty(context, JS_GetGlobalObject(context), "System", &jsParent);
    JS_GetProperty(context, JSVAL_TO_OBJECT(jsParent), "IO", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        context, parent, NULL, &File_class,
        File_constructor, 2, NULL, File_methods, NULL, File_static_methods
    );

    if (!object)
        return 0;

    JSObject* file;
    FileInformation* data;

    file = JS_DefineObject(
        context, parent,
        "STDIN", &File_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );
    data = (FileInformation*) malloc(sizeof(FileInformation));
    data->name       = strdup("stdin");
    data->descriptor = stdin;
    data->mode       = NULL;
    JS_SetPrivate(context, file, data);

    file = JS_DefineObject(
        context, parent,
        "STDOUT", &File_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );
    data = (FileInformation*) malloc(sizeof(FileInformation));
    data->name       = strdup("stdout");
    data->descriptor = stdout;
    data->mode       = NULL;
    JS_SetPrivate(context, file, data);
  
    file = JS_DefineObject(
        context, parent,
        "STDERR", &File_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );
    data = (FileInformation*) malloc(sizeof(FileInformation));
    data->name       = strdup("stderr");
    data->descriptor = stderr;
    data->mode       = NULL;
    JS_SetPrivate(context, file, data);

    return 1;
}

void
File_finalize (JSContext* context, JSObject* object)
{
    FileInformation* data = JS_GetPrivate(context, object);

    if (data) {
        if (data->name) {
            free(data->name);
        }

        if (data->mode) {
            fclose(data->descriptor);
            free(data->mode);
        }
        free(data);
    }
}

JSBool
File_constructor (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    return JS_TRUE;
}

JSBool
File_static_exists (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    return JS_TRUE;
}

