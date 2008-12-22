#include "Misc.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>

#define LOCAL  1
#define GLOBAL 2

const char* preprocess (const char* source, const char* fileName);
const char* include (const char* from, const char* fileName, int type);
const char* import (const char* path);

