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

    // Create STDIN special File object.
    file = JS_DefineObject(
        context, parent,
        "STDIN", &File_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    ); JS_DefineFunctions(context, file, File_methods);

    data = (FileInformation*) malloc(sizeof(FileInformation));
    data->name       = strdup("stdin");
    data->descriptor = stdin;
    data->mode       = NULL;
    JS_SetPrivate(context, file, data);

    // Create STDOUT special File object.
    file = JS_DefineObject(
        context, parent,
        "STDOUT", &File_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    ); JS_DefineFunctions(context, file, File_methods);

    data = (FileInformation*) malloc(sizeof(FileInformation));
    data->name       = strdup("stdout");
    data->descriptor = stdout;
    data->mode       = NULL;
    JS_SetPrivate(context, file, data);
  
    // Create STDERR special file object.
    file = JS_DefineObject(
        context, parent,
        "STDERR", &File_class, NULL,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    ); JS_DefineFunctions(context, file, File_methods);

    data = (FileInformation*) malloc(sizeof(FileInformation));
    data->name       = strdup("stderr");
    data->descriptor = stderr;
    data->mode       = NULL;
    JS_SetPrivate(context, file, data);

    // Default properties
    jsval property;

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
    const char* fileName;
    const char* mode;

    if (argc != 2 || !JS_ConvertArguments(context, argc, argv, "ss", &fileName, &mode)) {
        JS_ReportError(context, "File requires the path and the mode as arguments.");
        return JS_FALSE;
    }

    FileInformation* data = malloc(sizeof(FileInformation));
    data->name       = strdup(fileName);
    data->mode       = strdup(mode);
    data->descriptor = fopen(data->name, data->mode);
    JS_SetPrivate(context, object, data);

    if (!data->descriptor) {
        JS_ReportError(context, "An error occurred while opening the file.");
        return JS_FALSE;
    }

    return JS_TRUE;
}

JSBool
File_write (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* passed;
    char*     string;

    if (argc != 1 || !JS_ConvertArguments(context, argc, argv, "o", &passed)) {
        return JS_FALSE;
    }

    FileInformation* data = JS_GetPrivate(context, object);

    if (strcmp(((JSClass*)JS_GET_CLASS(context, passed))->name, "Array") == 0) {
        jsval part;
        int   written = 0;

        do {
            JS_CallFunctionName(context, passed, "shift", 0, NULL, &part);

            if (JSVAL_IS_STRING(part)) {
                string = strdup(JS_GetStringBytes(JSVAL_TO_STRING(OBJECT_TO_JSVAL(passed))));
                written += fwrite(string, sizeof(*string), strlen(string), data->descriptor);
            }
            else if (JSVAL_IS_NUMBER(part)) {
                int piece = JSVAL_TO_INT(part);
                written += fwrite(&piece, sizeof(int), 1, data->descriptor);
            }
            else {
                JS_ReportError(context, "You can put only Strings and Numbers into the passed Array.");
                return JS_FALSE;
            }
        } while (!JSVAL_IS_NULL(part));

        *rval = INT_TO_JSVAL(written);
    }
    else if (strcmp(((JSClass*)JS_GET_CLASS(context, passed))->name, "String") == 0) {
        jsval jsstr;
        jsval params[1]; params[0] = INT_TO_JSVAL(0);
        JS_CallFunctionName(context, passed, "substr", 1, params, &jsstr);

        string = strdup(JS_GetStringBytes(JSVAL_TO_STRING(jsstr)));
        *rval  = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), data->descriptor));
    }
    else {
        JS_ReportError(context, "You can pass only Strings and Arrays.");
        return JS_FALSE;
    }

    return JS_TRUE;
}

JSBool
File_read (JSContext *context, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int size;

    if (argc != 1 || !JS_ConvertArguments(context, argc, argv, "u", &size)) {
        JS_ReportError(context, "Not enough parameters.");
        return JS_FALSE;
    }

    FileInformation* data = JS_GetPrivate(context, object);

    if (feof(data->descriptor)) {
        *rval = JSVAL_FALSE;
        return JS_TRUE;
    }

    char* string = malloc(size*sizeof(char));
    fread(string, sizeof(char), size, data->descriptor);

    *rval = STRING_TO_JSVAL(JS_NewString(context, string, size));
    return JS_TRUE;
}

JSBool
File_isEnd (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    FileInformation* data = JS_GetPrivate(context, object);

    *rval = BOOLEAN_TO_JSVAL(feof(data->descriptor));
    return JS_TRUE;
}

JSBool
File_static_exists (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* name;

    if (argc != 1 || !JS_ConvertArguments(context, argc, argv, "s", &name)) {
        JS_ReportError(context, "Not enough parameters.");
        return JS_FALSE;
    }

    FILE* file = fopen(name, "r");
    if (file) {
        *rval = JSVAL_TRUE;
        fclose(file);
    }
    else {
        *rval = JSVAL_FALSE;
    }

    return JS_TRUE;
}

