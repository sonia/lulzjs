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
#include "jsapi.h"

#include "Misc.h"
#include "Core.h"

#include "Interactive.h"

char USAGE[] = {
    "lulzJS " __LJS_VERSION__ "\n"
    "\n"
    "    -v        Get lulzJS version.\n"
    "    -h        Read this help.\n"
};

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
    char* oneliner = NULL;
    int stopAt = argc;

    if (argc > 1) {
        int i;
        char prev = '\0';
        for (i = 1; i < argc; i++) {
            if (argv[i][0] != '-' && prev != 'e') {
                stopAt = i;
                break;
            }

            if (strlen(argv[i]) >= 2) {
                if (argv[i][0] == '-') {
                    prev = argv[i][1];
                }
            }
        }
    }

    int cmd;
    while ((cmd = getopt(stopAt, argv, "vVhe:")) != -1) {
        switch (cmd) {
            case 'V':
            puts("lulzJS " __LJS_VERSION__);
            return 0;
            break;

            case 'e':
            oneliner = optarg;
            break;

            default:
            puts(USAGE);
            return 0;
        }
    }

    Engine engine = initEngine(argc, (argc == 1 ? 0 : optind), argv);
    if (engine.error) {
        fprintf(stderr, "An error occurred while initializing the system.\n");
        return 1;
    }

    if (argc == 1) {
        Interactive(engine.context, engine.core);
    }
    else if (oneliner) {
        jsval rval = JSVAL_NULL;

        if (!JS_EvaluateScript(engine.context, engine.core, oneliner, strlen(oneliner), "lulzJS", 0, &rval)) {
            JS_ReportPendingException(engine.context);
        }
        else {
            printf("%s\n", JS_GetStringBytes(JS_ValueToString(engine.context, rval)));
        }
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
initEngine (int argc, int offset, char *argv[])
{
    Engine engine;
    engine.error = JS_TRUE;

    if (engine.runtime = JS_NewRuntime(8L * 1024L * 1024L)) {
        if (engine.context = JS_NewContext(engine.runtime, 8192)) {
            JS_SetOptions(engine.context, JSOPTION_VAROBJFIX);
            JS_SetErrorReporter(engine.context, reportError);

            if (engine.core = Core_initialize(engine.context, argv[offset])) {
                jsval property;
                JSObject* arguments = JS_NewArrayObject(engine.context, 0, NULL);
                property = OBJECT_TO_JSVAL(arguments);
                JS_SetProperty(engine.context, engine.core, "arguments", &property);

                if (offset+1 < argc) {
                    int i;
                    jsval rval;
                    for (i = offset+1; i < argc; i++) {
                        property = STRING_TO_JSVAL(JS_NewString(engine.context, JS_strdup(engine.context, argv[i]), strlen(argv[i])));
                        JS_CallFunctionName(
                            engine.context, arguments, "push",
                            1, &property, &rval);
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

