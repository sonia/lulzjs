#include <js/jsapi.h>
#include "Misc.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <dlfcn.h>

#define LOCAL  1
#define GLOBAL 2

const char* preprocess (JSContext* context, const char* source, const char* fileName);
short include (JSContext* context, const char* from, const char* fileName, int type);
short import (JSContext* context, const char* path);

