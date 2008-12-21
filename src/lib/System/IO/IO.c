#include "IO.h"

JSObject*
IO_initialize (JSContext* context, JSObject* parent)
{
    JSObject* object = JS_DefineObject(
        context, parent,
        IO_class.name, &IO_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (!object)
        return NULL;

    JS_DefineProperty(context, object, "STDIN",  INT_TO_JSVAL(fileno(stdin)),  NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDOUT", INT_TO_JSVAL(fileno(stdout)), NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDERR", INT_TO_JSVAL(fileno(stderr)), NULL, NULL, JSPROP_READONLY);

    JS_DefineFunctions(context, object, IO_methods);

    return object;
}

JSBool
IO_write (JSContext *context, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int fd;
    const char*        string;

    if (!JS_ConvertArguments(context, argc, argv, "us", &fd, &string)) {
        return JS_FALSE;
    }
    if (argc != 2) {
        return JS_TRUE;
    }

    FILE* fp;
    switch (fd) {
        case  0: fp = stdin;            break;
        case  1: fp = stdout;           break;
        case  2: fp = stderr;           break;
        default: fp = fdopen(fd, "r+"); break;
    }

    *rval = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), fp));
    return JS_TRUE;
}

