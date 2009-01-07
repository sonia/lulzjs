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

#include "Hash.h"

Pair*
Pair_create (const char* key, void* value)
{
    Pair* pair  = malloc(sizeof(Pair));
    pair->key   = strdup(key);
    pair->value = value;

    return pair;
}

void
Pair_destroy (Pair* pair)
{
    free(pair->key);
    free(pair);
}

Hash*
Hash_create ()
{
    Hash* hash = malloc(sizeof(Hash));
    hash[0] = NULL;
    return hash;
}

void
Hash_destroy (Hash* hash)
{
    size_t length = Hash_length(hash);

    size_t i;
    for (i = 0; i < length; i++) {
        Pair_destroy(hash[i]);
    }

    free (hash);
}

size_t
Hash_exists (Hash* hash, const char* key)
{
    size_t i;
    for (i = 0; i < Hash_length(hash); i++) {
        printf(":%d: %d\n", i, ((Pair*)hash[i])->key);
        printf("> %s\n\n", ((Pair*)hash[i])->key);
        if (strcmp(((Pair*)hash[i])->key, key) == 0) {
            return i+1;
        }
    }

    return HASH_FALSE;
}

size_t
Hash_length (Hash* hash)
{
    size_t i;
    for (i = 0; hash[i] != NULL; i++);
    return i;
}

void
Hash_set (Hash* hash, const char* key, void* value)
{
    size_t result = Hash_exists(hash, key);

    if (result == HASH_FALSE) {
        size_t length = Hash_length(hash);
        hash = realloc(hash, (length+2)*sizeof(Hash));
        hash[length] = Pair_create(key, value);
        hash[length+1] = NULL;
    }
    else {
        free(Hash_replaceIndex(hash, result-1, Pair_create(key, value)));
    }
}

void
Hash_setIndex (Hash* hash, size_t index, Pair* pair)
{
    size_t length = Hash_length(hash);

    if (index >= length || index < 0) {
        return;
    }

    free(hash[index]);
    hash[index] = pair;
}

void
Hash_setPair (Hash* hash, Pair* pair)
{
    size_t result = Hash_exists(hash, pair->key);

    if (result == HASH_FALSE) {
        size_t length = Hash_length(hash);
        hash = realloc(hash, (length+2)*sizeof(Hash));
        hash[length] = pair;
        hash[length+1] = NULL;
    }
    else {
        free(Hash_replaceIndex(hash, result-1, pair));
    }
}

void*
Hash_get (Hash* hash, const char* key)
{
    size_t result = Hash_exists(hash, key);

    if (result == HASH_FALSE) {
        return NULL;
    }

    return Hash_getIndex(hash, result-1);
}

void*
Hash_getIndex (Hash* hash, size_t index)
{
    size_t length = Hash_length(hash);

    if (index >= length || index < 0) {
        return NULL;
    }

    return ((Pair*)hash[index])->value;
}

Pair*
Hash_replace (Hash* hash, const char* key, void* value)
{
    size_t result = Hash_exists(hash, key);

    if (result == HASH_FALSE) {
        return NULL;
    }

    return Hash_replaceIndex(hash, result-1, Pair_create(key, value));
}

Pair*
Hash_replaceIndex (Hash* hash, size_t index, Pair* pair)
{
    size_t length = Hash_length(hash);

    if (index >= length || index < 0) {
        return NULL;
    }

    Pair* returned = hash[index];
    hash[index]    = pair;

    return returned;
}

Pair*
Hash_replacePair (Hash* hash, Pair* pair)
{
    size_t result = Hash_exists(hash, pair->key);

    if (result == HASH_FALSE) {
        return NULL;
    }

    return Hash_replaceIndex(hash, result-1, pair);
}

void
Hash_delete (Hash* hash, const char* key)
{
    Hash_deleteIndex(hash, Hash_exists(hash, key));
}

void
Hash_deleteIndex (Hash* hash, size_t index)
{
    size_t length = Hash_length(hash);

    if (index >= length || index < 0) {
        return;
    }

    Pair_destroy(hash[index]);

    size_t i;
    for (i = index; i < length-1; i++) {
        hash[i] = hash[i+1];
    }

    hash = realloc(hash, (length)*sizeof(Hash));
    hash[length-1] = NULL;
}

void
Hash_deletePair (Hash* hash, Pair* pair)
{
    Hash_deleteIndex(hash, Hash_exists(hash, pair->key));
}
