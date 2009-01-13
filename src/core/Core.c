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

#include "Core.h"

extern Hash* timeouts;
extern Hash* intervals;

JSObject*
Core_initialize (JSContext *cx, const char* script)
{
    srand((unsigned) time(NULL));

    // FIXME: Also these need to use garbage collection.
    timeouts  = Hash_create();
    intervals = Hash_create();

    JSObject* object = JS_NewObject(cx, &Core_class, NULL, NULL);

    if (object && JS_InitStandardClasses(cx, object)) {
        JS_DefineFunctions(cx, object, Core_methods);

        // Properties
        jsval property;

        char* rootPath = __Core_getRootPath(cx, script);
        jsval paths[] = {
            STRING_TO_JSVAL(JS_NewString(cx, rootPath, strlen(rootPath))),
            STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, __LJS_LIBRARY_PATH__), strlen(__LJS_LIBRARY_PATH__)))
        };
        JSObject* path = JS_NewArrayObject(cx, 2, paths);
        property       = OBJECT_TO_JSVAL(path);
        JS_SetProperty(cx, object, "__PATH__", &property);

        property = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, __LJS_VERSION__), strlen(__LJS_VERSION__)));
        JS_SetProperty(cx, object, "__VERSION__", &property);

        property = OBJECT_TO_JSVAL(object);
        JS_SetProperty(cx, object, "Program", &property);

        property = STRING_TO_JSVAL(JS_NewString(cx, JS_strdup(cx, script), strlen(script)));
        JS_SetProperty(cx, object, "name", &property);

        if (__Core_include(cx, __LJS_LIBRARY_PATH__ "/Core"))
            return object;
    }

    return NULL;
}

JSBool
Core_include (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject* files;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &files)) {
        JS_ReportError(cx, "You must pass only the modules/files to include.");
        return JS_FALSE;
    }

    if (JS_IsArrayObject(cx, files)) {
        JSObject* retArray = JS_NewArrayObject(cx, 0, NULL);

        jsuint length;
        JS_GetArrayLength(cx, files, &length);

        jsuint i;
        for (i = 0; i < length; i++) {
            jsval fileName;
            JS_GetElement(cx, files, i, &fileName);

            char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(fileName))));
            jsval ret = BOOLEAN_TO_JSVAL(__Core_include(cx, path));

            JS_SetElement(cx, retArray, i, &ret);
            JS_free(cx, path);
        }

        *rval = OBJECT_TO_JSVAL(retArray);
    }
    else {
        char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        *rval = BOOLEAN_TO_JSVAL(__Core_include(cx, path));
        JS_free(cx, path);
    }

    return JS_TRUE;
}

JSBool
Core_require (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject* files;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &files)) {
        JS_ReportError(cx, "You must pass only the modules/files to include.");
        return JS_FALSE;
    }

    short ok;

    if (JS_IsArrayObject(cx, files)) {
        jsuint length;
        JS_GetArrayLength(cx, files, &length);

        size_t i;
        for (i = 0; i < length; i++) {
            jsval fileName;
            JS_GetElement(cx, files, i, &fileName);

            char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(fileName))));
            if (!__Core_include(cx, path)) {
                JS_ReportError(cx, "%s couldn't be included.", path);
                JS_free(cx, path);

                return JS_FALSE;
            }
            JS_free(cx, path);
        }
    }
    else {
        char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        ok = __Core_include(cx, path);

        if (!ok) {
            JS_ReportError(cx, "%s couldn't be included.", path);
            JS_free(cx, path);

            return JS_FALSE;
        }
        JS_free(cx, path);
    }

    return JS_TRUE;
}

JSBool
Core_GC (JSContext* cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JS_MaybeGC(cx);

    return JS_TRUE;
}

JSBool
Core_die (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    char* error;

    if (argc) {
        error = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
    }
    else {
        error = JS_strdup(cx, "Program died.");
    }

    JS_ReportError(cx, error);
    JS_ReportPendingException(cx);

    exit(EXIT_FAILURE);
    return JS_FALSE;
}

JSBool
Core_exit (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    exit(EXIT_SUCCESS);
    return JS_FALSE;
}

JSBool
Core_ENV (JSContext* cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    char* env;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &env)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    char* value = getenv(env);

    if (value == NULL) {
        *rval = JSVAL_NULL;
    }
    else {
        value = JS_strdup(cx, value);
        *rval = STRING_TO_JSVAL(JS_NewString(cx, value, strlen(value)));
    }

    return JS_TRUE;
}

JSBool
Core_exec (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    FILE* pipe;
    char* output  = NULL;
    size_t length = 0;
    size_t read   = 0;
    const char* command;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &command)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }
    
    if ((pipe = popen(command, "r")) == NULL) {
        JS_ReportError(cx, "Command not found");
        return JS_FALSE;
    }

    // Read untill the pipe is empty.
    while (1) {
        output = JS_realloc(cx, output, length+=512);
        read   = fread(output+(length-512), sizeof(char), 512, pipe);

        if (read < 512) {
            output = JS_realloc(cx, output, length-=(512-read));
            break;
        }
    }
    output[length-1] = '\0';
    pclose(pipe);

    *rval = STRING_TO_JSVAL(JS_NewString(cx, output, length));
    return JS_TRUE;
}


char*
__Core_getRootPath (JSContext* cx, const char* fileName)
{
    char* path;

    if (fileName == NULL) {
        path = JS_strdup(cx, getenv("PWD"));
    }
    else if (fileName[0] == '/') {
        path = dirname(JS_strdup(cx, fileName));
    }
    else {
        path = JS_strdup(cx, getenv("PWD"));
        path = JS_realloc(cx, path, (strlen(path)+2)*sizeof(char));
        strcat(path, "/");
        path = JS_realloc(cx, path, (strlen(path)+strlen(fileName)+1)*sizeof(char));
        strcat(path, fileName);
        path = dirname(path);
    }

    return path;
}

const char*
__Core_getScriptName (JSContext* cx)
{
    JSStackFrame* fp = NULL;
    fp = JS_FrameIterator(cx, &fp); fp = JS_FrameIterator(cx, &fp);
    JSScript* script = JS_GetFrameScript(cx, fp);

    return JS_GetScriptFilename(cx, script);
}

char*
__Core_getPath (JSContext* cx, const char* fileName)
{
    /*
     * Getting the dirname of the file from the other file is included
     * then copying it and getting the path to the dir.
     */
    char* from = JS_strdup(cx, __Core_getScriptName(cx));
    char* dir  = dirname(from);
    char* path = JS_malloc(cx, (strlen(dir)+2)*sizeof(char));

    strcpy(path, dir); strcat(path, "/"); JS_free(cx, from);
    
    /*
     * Copying the base to the path and then adding the relative path to
     * the file to import
     */
    path = JS_realloc(cx, path, (strlen(path)+strlen(fileName)+1)*sizeof(char));
    strcat(path, fileName);

    if (!fileExists(path)) {
        jsval jsPath;
        JS_GetProperty(cx, JS_GetGlobalObject(cx), "__PATH__", &jsPath);
        JSObject* lPath = JSVAL_TO_OBJECT(jsPath);

        jsuint length;
        JS_GetArrayLength(cx, lPath, &length);

        size_t i;
        for (i = 0; i < length; i++) {
            JS_free(cx, path);

            jsval pathFile;
            JS_GetElement(cx, lPath, i, &pathFile);

            path = JS_strdup(cx, JS_GetStringBytes(JSVAL_TO_STRING(pathFile)));
            path = JS_realloc(cx, path, (strlen(path)+2)*sizeof(char));
            strcat(path, "/");
            path = JS_realloc(cx, path, (strlen(path)+strlen(fileName)+1)*sizeof(char));
            strcat(path, fileName);

            if (fileExists(path)) {
                break;
            }
        }
    }

    return path;
}

JSBool
__Core_include (JSContext* cx, const char* path)
{
    if (__Core_isIncluded(path)) {
        #ifdef DEBUG
        printf("(already included) %s\n", path);
        #endif

        return JS_TRUE;
    }

    if (strstr(path, ".js") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(javascript) path: %s\n", path);
        #endif

        if (!fileExists(path)) {
            #ifdef DEBUG
            printf("(javascript) %s not found.\n", path);
            #endif
            return JS_FALSE;
        }

        jsval rval;
        char* sources = stripRemainder(cx, readFile(cx, path));

        JSBool result = (short) JS_EvaluateScript(cx, JS_GetGlobalObject(cx), sources, strlen(sources), path, 1, &rval);
        JS_free(cx, sources);

        while (JS_IsExceptionPending(cx)) {
            JS_ReportPendingException(cx);

            return JS_FALSE;
        }
    }
    else if (strstr(path, ".so") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(object) path: %s\n", path);
        #endif

        if (!fileExists(path)) {
            #ifdef DEBUG
            printf("(object) %s not found.\n", path);
            #endif
            return JS_FALSE;
        }

        void* handle = dlopen(path, RTLD_LAZY|RTLD_GLOBAL);
        char* error = dlerror();

        if (error) {
            fprintf(stderr, "%s\n", error);
            return JS_FALSE;
        }

        short (*exec)(JSContext*) = dlsym(handle, "exec");

        if(!(*exec)(cx)) {
            fprintf(stderr, "The initialization of the module failed.\n");
            return JS_FALSE;
        }
    }
    else {
        #ifdef DEBUG
        printf("(module) path: %s\n", path);
        #endif

        char* newPath = JS_strdup(cx, path);
        newPath = JS_realloc(cx, newPath, (strlen(newPath)+strlen("/init.js")+1)*sizeof(char));
        strcat(newPath, "/init.js");

        if (!__Core_include(cx, newPath)) {
            JS_free(cx, newPath);
            return JS_FALSE;
        }
        JS_free(cx, newPath);
    }

    included = JS_realloc(cx, included, ++includedNumber*sizeof(char*));
    included[includedNumber-1] = JS_strdup(cx, path);

    return JS_TRUE;
}

JSBool
__Core_isIncluded (const char* path)
{
    size_t i;
    for (i = 0; i < includedNumber; i++) {
        if (strcmp(included[i], path) == 0) {
            return JS_TRUE;
        }
    }

    return JS_FALSE;
}
