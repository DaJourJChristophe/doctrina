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
#ifndef SET_H
#define SET_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "internal/hash.h"

#include <stddef.h>
#include <stdint.h>

typedef struct bucket bucket_t;

struct set
{
  bucket_t **buckets;
    size_t   size;
};

typedef struct set set_t;

set_t *set_new(const size_t size);

void set_destroy(set_t *self);

void *set_getall(set_t *self, size_t *overall_size);

int set_exists(set_t *self, const void *key, const size_t keylen);

int set_add(set_t *self, const void *key, const size_t keylen);

int set_remove(set_t *self, const void *key, const size_t keylen);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*SET_H*/
