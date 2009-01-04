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

JSObject*
Core_initialize (JSContext *cx)
{
    JSObject* object = JS_NewObject(cx, &Core_class, NULL, NULL);

    if (object && JS_InitStandardClasses(cx, object)) {
        JS_DefineFunctions(cx, object, Core_methods);

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

        size_t i;
        for (i = 0; i < length; i++) {
            jsval fileName;
            JS_GetElement(cx, files, i, &fileName);

            char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(fileName))));
            jsval ret = BOOLEAN_TO_JSVAL(__Core_include(cx, path));

            JS_SetElement(cx, retArray, i, &ret);
            free(path);
        }

        *rval = OBJECT_TO_JSVAL(retArray);
    }
    else {
        char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        *rval = BOOLEAN_TO_JSVAL(__Core_include(cx, path));
        free(path);
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
                free(path);
                return JS_FALSE;
            }
            free(path);
        }
    }
    else {
        char* path = __Core_getPath(cx, JS_GetStringBytes(JS_ValueToString(cx, OBJECT_TO_JSVAL(files))));
        ok = __Core_include(cx, path);

        if (!ok) {
            JS_ReportError(cx, "%s couldn't be included.", path);
            free(path);
            return JS_FALSE;
        }
        free(path);
    }

    *rval = JSVAL_NULL;
    return JS_TRUE;
}

char*
__Core_getPath (JSContext* cx, const char* fileName)
{
    JSStackFrame* fp = NULL;
    fp = JS_FrameIterator(cx, &fp); fp = JS_FrameIterator(cx, &fp);
    JSScript* script = JS_GetFrameScript(cx, fp);

    /*
     * Getting the dirname of the file from the other file is included
     * then copying it and getting the path to the dir.
     */
    char* from = strdup(JS_GetScriptFilename(cx, script));
    char* dir  = dirname(from);
    char* path = malloc((strlen(dir)+2)*sizeof(char));

    strcpy(path, dir); strcat(path, "/"); free(from);
    
    /*
     * Copying the base to the path and then adding the relative path to
     * the file to import
     */
    path = realloc(path, (strlen(path)+strlen(fileName)+1)*sizeof(char));
    strcat(path, fileName);

    if (!fileExists(path)) {
        free(path);
        path = strdup(__LJS_LIBRARY_PATH__);
        path = realloc(path, (strlen(path)+2)*sizeof(char));
        strcat(path, "/");
        path = realloc(path, (strlen(path)+strlen(fileName)+1)*sizeof(char));
        strcat(path, fileName);
    }

    return path;
}

short
__Core_include (JSContext* cx, const char* path)
{
    if (strstr(path, ".js") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(javascript) path: %s\n", path);
        #endif

        if (!fileExists(path)) {
            #ifdef DEBUG
            printf("(javascript) %s not found.\n", path);
            #endif
            return 0;
        }

        jsval rval;
        char* sources = (char*) stripRemainder((char*) readFile(path));

        short result = (short) JS_EvaluateScript(cx, JS_GetGlobalObject(cx), sources, strlen(sources), path, 0, &rval);
        free(sources);
        return result;
    }
    else if (strstr(path, ".so") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(object) path: %s\n", path);
        #endif

        if (!fileExists(path)) {
            #ifdef DEBUG
            printf("(object) %s not found.\n", path);
            #endif
            return 0;
        }

        void* handle = dlopen(path, RTLD_LAZY|RTLD_GLOBAL);
        char* error = dlerror();

        if (error) {
            fprintf(stderr, "%s\n", error);
            return 0;
        }

        short (*exec)(JSContext*) = dlsym(handle, "exec");

        if(!(*exec)(cx)) {
            fprintf(stderr, "The initialization of the module failed.\n");
            return 0;
        }
    }
    else {
        #ifdef DEBUG
        printf("(module) path: %s\n", path);
        #endif

        char* newPath = strdup(path);
        newPath = realloc(newPath, (strlen(newPath)+strlen("/init.js")+1)*sizeof(char));
        strcat(newPath, "/init.js");

        short result = __Core_include(cx, newPath);
        free(newPath);
        return result;
    }

    return 1;
}

