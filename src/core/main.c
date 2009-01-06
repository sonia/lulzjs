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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "jsapi.h"

#include "Misc.h"
#include "Core.h"

#include "Interactive.h"

typedef struct {
    JSBool     error;
    JSRuntime* runtime;
    JSContext* context;
    JSObject*  core;
} Engine;

void reportError (JSContext *cx, const char *message, JSErrorReport *report);
Engine initEngine (int argc, int optind, char *argv[]);
JSBool executeScript (JSContext* cx, const char* file);

int
main (int argc, char *argv[])
{
    int cmd;
    while ((cmd = getopt(argc, argv, "v")) != -1) {
        switch (cmd) {
            case 'v':
            puts("lulzJS " __LJS_VERSION__);
            return 0;
            break;
        }
    }

    Engine engine = initEngine(argc, optind, argv);
    if (engine.error) {
        fprintf(stderr, "An error occurred while initializing the system.\n");
        return 1;
    }

    if (argc == 1) {
        Interactive(engine.context, engine.core);
    }
    else {
        if (!fileExists(argv[optind])) {
            fprintf(stderr, "The file doesnt't exist.\n");
            return 1;
        }

        if (!executeScript(engine.context, argv[optind])) {
            fprintf(stderr, "The script couldn't be executed.\n");
            return 1;
        }
    }
    
    JS_DestroyContext(engine.context);
    JS_DestroyRuntime(engine.runtime);
    JS_ShutDown();

    return 0;
}

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u > %s\n",
        report->filename ? report->filename : "<no filename>",
        (unsigned int) report->lineno,
        message
    );
}

Engine
initEngine (int argc, int optind, char *argv[])
{
    Engine engine;
    engine.error = JS_TRUE;

    if (engine.runtime = JS_NewRuntime(8L * 1024L * 1024L)) {
        if (engine.context = JS_NewContext(engine.runtime, 8192)) {
            JS_SetOptions(engine.context, JSOPTION_VAROBJFIX);
            JS_SetErrorReporter(engine.context, reportError);

            if (engine.core = Core_initialize(engine.context, argv[optind])) {
                jsval pass;
                JSObject* arguments = JS_NewArrayObject(engine.context, 0, NULL);
                pass = OBJECT_TO_JSVAL(arguments);
                JS_SetProperty(engine.context, engine.core, "arguments", &pass);

                if (optind < argc) {
                    int i;
                    jsval rval;
                    for (i = optind; i < argc; i++) {
                        pass = STRING_TO_JSVAL(JS_NewString(engine.context, JS_strdup(engine.context, argv[i]), strlen(argv[i])));
                        JS_CallFunctionName(
                            engine.context, arguments, "push",
                            1, &pass, &rval);
                    }
                }

                engine.error = JS_FALSE;
                return engine;
            }
        }
    }

    return engine;
}

JSBool
executeScript (JSContext* cx, const char* file)
{
    JSBool    returnValue;
    jsval     rval;
    JSObject* global = JS_GetGlobalObject(cx);

    char* sources = stripRemainder(cx, readFile(cx, file));
    returnValue = JS_EvaluateScript(cx, global, sources, strlen(sources), file, 0, &rval);
    JS_free(cx, sources);

    return returnValue;
}

