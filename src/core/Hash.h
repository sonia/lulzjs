/****************************************************************************
* Copyleft meh.                                                             *
*                                                                           *
* This is free software: you can redistribute it and/or modif.              *
* it under the terms of the GNU Affero General Public License a.            *
* published by the Free Software Foundation, either version 3 of the        *
* License, or (at your option) any later version.                           *
*                                                                           *
* This program is distributed in the hope that it will be useful.           *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU Affero General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Affero General Public License  *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
****************************************************************************/

#ifndef _HASH_H
#define _HASH_H

#include <stdlib.h>
#include <string.h>

#define HASH_FALSE 0
#define HASH_TRUE  !HASH_FALSE

typedef struct {
    char* key;
    void* value;
} Pair;

typedef Pair* Hash;

Pair* Pair_create  (const char* key, void* value);
void  Pair_destroy (Pair* pair);

Hash* Hash_create  ();
void  Hash_destroy (Hash* hash);

size_t Hash_exists (Hash* hash, const char* key);
size_t Hash_length (Hash* hash);

void Hash_set      (Hash* hash, const char* key, void* value);
void Hash_setIndex (Hash* hash, size_t index, Pair* pair);
void Hash_setPair  (Hash* hash, Pair* pair);

void* Hash_get      (Hash* hash, const char* key);
void* Hash_getIndex (Hash* hash, size_t index);

Pair* Hash_replace      (Hash* hash, const char* key, void* value);
Pair* Hash_replaceIndex (Hash* hash, size_t index, Pair* pair);
Pair* Hash_replacePair  (Hash* hash, Pair* pair);

void Hash_delete      (Hash* hash, const char* key);
void Hash_deleteIndex (Hash* hash, size_t index);
void Hash_deletePair  (Hash* hash, Pair* pair);


#endif
