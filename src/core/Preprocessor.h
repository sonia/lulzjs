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

#include "jsapi.h"
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

