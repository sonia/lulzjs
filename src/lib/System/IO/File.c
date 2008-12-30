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

extern short exec (JSContext* context) { return File_initialize(context); }

short
File_initialize (JSContext* context)
{
    jsval jsParent;
    JS_GetProperty(context, JS_GetGlobalObject(context), "System", &jsParent);
    JS_GetProperty(context, JSVAL_TO_OBJECT(jsParent), "IO", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    jsval jsSuper;
    JS_GetProperty(context, parent, "Stream", &jsSuper);

    JSObject* object = JS_InitClass(
        context, parent, JSVAL_TO_OBJECT(jsSuper), &File_class,
        File_constructor, 2, NULL, File_methods, NULL, File_static_methods
    );

    if (!object)
        return 0;

    // Default properties
    jsval property;

    return 1;
}

void
File_finalize (JSContext* context, JSObject* object)
{
    FileInformation* data = JS_GetPrivate(context, object);

    if (data) {
        if (data->path) {
            free(data->path);
        }

        if (data->mode) {
            fclose(data->stream->descriptor);
            free(data->mode);
        }

        if (data->stream) {
            free(data->stream);
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

    FileInformation* data    = malloc(sizeof(FileInformation));
    data->path               = strdup(fileName);
    data->mode               = strdup(mode);
    data->stream             = (StreamInformation*) malloc(sizeof(StreamInformation));
    data->stream->descriptor = fopen(data->path, data->mode);
    JS_SetPrivate(context, object, data);

    if (!data->stream->descriptor) {
        JS_ReportError(context, "An error occurred while opening the file.");
        return JS_FALSE;
    }

    return JS_TRUE;
}

JSBool
File_write (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    if (argc != 1 || !JS_ConvertArguments(context, argc, argv, "s", &string)) {
        return JS_FALSE;
    }

    FileInformation* data = JS_GetPrivate(context, object);

    *rval = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), data->stream->descriptor));
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

    if (feof(data->stream->descriptor)) {
        *rval = JSVAL_FALSE;
        return JS_TRUE;
    }

    char* string = malloc(size*sizeof(char));
    fread(string, sizeof(char), size, data->stream->descriptor);

    *rval = STRING_TO_JSVAL(JS_NewString(context, string, size));
    return JS_TRUE;
}

JSBool
File_isEnd (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    FileInformation* data = JS_GetPrivate(context, object);

    *rval = BOOLEAN_TO_JSVAL(feof(data->stream->descriptor));
    return JS_TRUE;
}

JSBool
File_static_exists (JSContext* context, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* path;

    if (argc != 1 || !JS_ConvertArguments(context, argc, argv, "s", &path)) {
        JS_ReportError(context, "Not enough parameters.");
        return JS_FALSE;
    }

    FILE* file = fopen(path, "r");
    if (file) {
        *rval = JSVAL_TRUE;
        fclose(file);
    }
    else {
        *rval = JSVAL_FALSE;
    }

    return JS_TRUE;
}

