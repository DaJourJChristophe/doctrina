/*
 * Copyright (C) 2025 Da'Jour J. Christophe. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "internal/hash.h"

#include <stddef.h>
#include <stdint.h>

typedef struct bucket bucket_t;

struct map
{
  bucket_t **buckets;
    size_t   size;
};

typedef struct map map_t;

map_t *map_new(const size_t size);

void map_destroy(map_t *self);

void *map_get(map_t *self, const void *key, const size_t keylen, size_t *size);

int map_exists(map_t *self, const void *key, const size_t keylen);

int map_set(map_t *self, const void *key,  const size_t keylen,
                         const void *data, const size_t datalen);

int map_del(map_t *self, const void *key, const size_t keylen);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*MAP_H*/
