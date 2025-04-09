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
#ifndef COMMON_H
#define COMMON_H

#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif/*UNUSED*/

#ifndef always_inline
#define always_inline __attribute__ ((always_inline))
#endif/*always_inline*/

#ifndef unused
#define unused __attribute__ ((unused))
#endif/*unused*/

#endif/*COMMON_H*/

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
#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>

uint64_t xxh3(const void *data, size_t len, uint64_t seed);

#define __hash__(data, len, seed) xxh3(data, len, seed)

#endif/*HASH_H*/

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

//-----------------------------------------------------------------------------
// xxHash Library
// Copyright (c) 2012-2021 Yann Collet
// All rights reserved.
//
// BSD 2-Clause License (https://www.opensource.org/licenses/bsd-license.php)
//
// xxHash3
//-----------------------------------------------------------------------------
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define XXH_PRIME_1 11400714785074694791ULL
#define XXH_PRIME_2 14029467366897019727ULL
#define XXH_PRIME_3 1609587929392839161ULL
#define XXH_PRIME_4 9650029242287828579ULL
#define XXH_PRIME_5 2870177450012600261ULL

static uint64_t XXH_read64(const void *memptr)
{
  uint64_t val;
  memcpy(&val, memptr, sizeof(val));
  return val;
}

static uint32_t XXH_read32(const void *memptr)
{
  uint32_t val;
  memcpy(&val, memptr, sizeof(val));
  return val;
}

static uint64_t XXH_rotl64(uint64_t x, int r)
{
  return (x << r) | (x >> (64 - r));
}

uint64_t xxh3(const void *data, size_t len, uint64_t seed)
{
  const uint8_t *p = (const uint8_t *)data;
  const uint8_t * const end = p + len;
  uint64_t h64;

  if (len >= 32)
  {
    const uint8_t * const limit = end - 32;
    uint64_t v1 = seed + XXH_PRIME_1 + XXH_PRIME_2;
    uint64_t v2 = seed + XXH_PRIME_2;
    uint64_t v3 = seed + 0;
    uint64_t v4 = seed - XXH_PRIME_1;

    do
    {
      v1 += XXH_read64(p) * XXH_PRIME_2;
      v1 = XXH_rotl64(v1, 31);
      v1 *= XXH_PRIME_1;

      v2 += XXH_read64(p + 8) * XXH_PRIME_2;
      v2 = XXH_rotl64(v2, 31);
      v2 *= XXH_PRIME_1;

      v3 += XXH_read64(p + 16) * XXH_PRIME_2;
      v3 = XXH_rotl64(v3, 31);
      v3 *= XXH_PRIME_1;

      v4 += XXH_read64(p + 24) * XXH_PRIME_2;
      v4 = XXH_rotl64(v4, 31);
      v4 *= XXH_PRIME_1;

      p += 32;
    } while (p <= limit);

    h64 = XXH_rotl64(v1,  1) + XXH_rotl64(v2,  7)
        + XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);

    v1 *= XXH_PRIME_2;
    v1 = XXH_rotl64(v1, 31);
    v1 *= XXH_PRIME_1;
    h64 ^= v1;
    h64 = h64 * XXH_PRIME_1 + XXH_PRIME_4;

    v2 *= XXH_PRIME_2;
    v2 = XXH_rotl64(v2, 31);
    v2 *= XXH_PRIME_1;
    h64 ^= v2;
    h64 = h64 * XXH_PRIME_1 + XXH_PRIME_4;

    v3 *= XXH_PRIME_2;
    v3 = XXH_rotl64(v3, 31);
    v3 *= XXH_PRIME_1;
    h64 ^= v3;
    h64 = h64 * XXH_PRIME_1 + XXH_PRIME_4;

    v4 *= XXH_PRIME_2;
    v4 = XXH_rotl64(v4, 31);
    v4 *= XXH_PRIME_1;
    h64 ^= v4;
    h64 = h64 * XXH_PRIME_1 + XXH_PRIME_4;
  }
  else
  {
    h64 = seed + XXH_PRIME_5;
  }

  h64 += (uint64_t)len;

  while (p + 8 <= end)
  {
    uint64_t k1 = XXH_read64(p);
    k1 *= XXH_PRIME_2;
    k1 = XXH_rotl64(k1, 31);
    k1 *= XXH_PRIME_1;
    h64 ^= k1;
    h64 = XXH_rotl64(h64, 27) * XXH_PRIME_1 + XXH_PRIME_4;
    p += 8;
  }

  if (p + 4 <= end)
  {
    h64 ^= (uint64_t)(XXH_read32(p)) * XXH_PRIME_1;
    h64 = XXH_rotl64(h64, 23) * XXH_PRIME_2 + XXH_PRIME_3;
    p += 4;
  }

  while (p < end)
  {
    h64 ^= (*p) * XXH_PRIME_5;
    h64 = XXH_rotl64(h64, 11) * XXH_PRIME_1;
    p++;
  }

  h64 ^= h64 >> 33;
  h64 *= XXH_PRIME_2;
  h64 ^= h64 >> 29;
  h64 *= XXH_PRIME_3;
  h64 ^= h64 >> 32;

  return h64;
}

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

//#include "internal/hash.h"

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
//#include "internal/hash.h"
//#include "common.h"
//#include "map.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bucket
{
  size_t   keylen;
  size_t   size;
  uint8_t *key;
  uint8_t *data;
};

static bucket_t unused *bucket_empty(void)
{
  bucket_t *self = NULL;

  self = (bucket_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket to the heap");
    exit(EXIT_FAILURE);
  }

  return self;
}

static bucket_t *bucket_new(const void *key, const size_t keylen, const void *data, const size_t size)
{
  bucket_t *self = NULL;

  self = (bucket_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket to the heap");
    exit(EXIT_FAILURE);
  }

  if (key != NULL && keylen > 0UL)
  {
    self->key = (uint8_t *)calloc(keylen, sizeof(*self->key));
    if (self->key == NULL)
    {
      fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket.key to the heap");
      exit(EXIT_FAILURE);
    }
    memcpy(self->key, key, keylen);
    self->keylen = keylen;
  }

  if (data != NULL && size > 0UL)
  {
    self->data = (uint8_t *)calloc(size, sizeof(*self->data));
    if (self->data == NULL)
    {
      fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket.data to the heap");
      exit(EXIT_FAILURE);
    }
    memcpy(self->data, data, size);
    self->size = size;
  }

  return self;
}

static void bucket_destroy(bucket_t **self)
{
  if (self != NULL && *self != NULL)
  {
    if ((*self)->key != NULL)
    {
      free((*self)->key);
      (*self)->key = NULL;
    }

    if ((*self)->data != NULL)
    {
      free((*self)->data);
      (*self)->data = NULL;
    }

    free(*self);
    *self = NULL;
  }
}

static void bucket_update(bucket_t *self, const void *data, const size_t size)
{
  if (size != self->size)
  {
    void *old = NULL;

    old = self->data;
    self->data = NULL;

    self->data = (uint8_t *)realloc(old, size);
    if (self->data == NULL)
    {
      fprintf(stderr, "%s(): %s\n", __func__, "could not re- allocate bucket.data to the heap");
      exit(EXIT_FAILURE);
    }
  }

  memcpy(self->data, data, size);
}

static void *bucket_data(bucket_t *self)
{
  void *data = NULL;

  data = calloc(self->size, sizeof(*self->data));
  if (self->data == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate buffer to the heap");
    exit(EXIT_FAILURE);
  }

  memcpy(data, self->data, self->size);

  return data;
}

static size_t bucket_size(bucket_t *self)
{
  return self->size;
}

static int bucket_haskey(bucket_t *self, const void *key, const size_t keylen)
{
  return self->keylen == keylen && memcmp(self->key, key, keylen) == 0;
}

map_t *map_new(const size_t size)
{
  map_t *self = NULL;

  self = (map_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate map to the heap");
    exit(EXIT_FAILURE);
  }

  self->buckets = (bucket_t **)calloc(size, sizeof(*self->buckets));
  if (self->buckets == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate map.buckets to the heap");
    exit(EXIT_FAILURE);
  }

  self->size = size;

  return self;
}

void map_destroy(map_t *self)
{
  if (self != NULL)
  {
    if (self->buckets != NULL)
    {
      uint64_t i;

      for (i = 0UL; i < self->size; i++)
      {
        if (self->buckets[i] == NULL)
        {
          continue;
        }

        bucket_destroy(&self->buckets[i]);
      }

      free(self->buckets);
      self->buckets = NULL;
    }

    free(self);
    self = NULL;
  }
}

#define SEED 2

void *map_get(map_t *self, const void *key, const size_t keylen, size_t *size)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  *size = 0UL;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] == NULL)
    {
      return NULL;
    }

    if (0 == bucket_haskey(self->buckets[j], key, keylen))
    {
      continue;
    }

    *size = bucket_size(self->buckets[j]);
    return bucket_data(self->buckets[j]);
  }

  return NULL;
}

int map_exists(map_t *self, const void *key, const size_t keylen)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] == NULL)
    {
      return 0;
    }

    if (0 == bucket_haskey(self->buckets[j], key, keylen))
    {
      continue;
    }

    return 1;
  }

  return 0;
}

int map_set(map_t *self, const void *key,  const size_t keylen,
                         const void *data, const size_t datalen)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] != NULL)
    {
      if (1 == bucket_haskey(self->buckets[j], key, keylen))
      {
        bucket_update(self->buckets[j], data, datalen);
        return 0;
      }

      continue;
    }

    self->buckets[j] = bucket_new(key, keylen, data, datalen);
    return 0;
  }

  return (-1);
}

int map_del(map_t *self, const void *key, const size_t keylen)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] == NULL)
    {
      return (-1);
    }

    if (0 == bucket_haskey(self->buckets[j], key, keylen))
    {
      continue;
    }

    bucket_destroy(&self->buckets[j]);
    return 0;
  }

  return (-1);
}

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

//#include "internal/hash.h"

#include <stddef.h>
#include <stdint.h>

typedef struct set_bucket set_bucket_t;

struct set
{
  set_bucket_t **buckets;
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
//#include "internal/hash.h"
//#include "common.h"
//#include "set.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct set_bucket
{
  size_t   keylen;
  uint8_t *key;
};

static set_bucket_t unused *set_bucket_empty(void)
{
  set_bucket_t *self = NULL;

  self = (set_bucket_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket to the heap");
    exit(EXIT_FAILURE);
  }

  return self;
}

static set_bucket_t *set_bucket_new(const void *key, const size_t keylen)
{
  set_bucket_t *self = NULL;

  self = (set_bucket_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket to the heap");
    exit(EXIT_FAILURE);
  }

  if (key != NULL && keylen > 0UL)
  {
    self->key = (uint8_t *)calloc(keylen, sizeof(*self->key));
    if (self->key == NULL)
    {
      fprintf(stderr, "%s(): %s\n", __func__, "could not allocate bucket.key to the heap");
      exit(EXIT_FAILURE);
    }
    memcpy(self->key, key, keylen);
    self->keylen = keylen;
  }

  return self;
}

static void set_bucket_destroy(set_bucket_t **self)
{
  if (self != NULL && *self != NULL)
  {
    if ((*self)->key != NULL)
    {
      free((*self)->key);
      (*self)->key = NULL;
    }

    free(*self);
    *self = NULL;
  }
}

static void *set_bucket_key(set_bucket_t *self, size_t *size)
{
  void *key = NULL;

  key = calloc(self->keylen, sizeof(*self->key));
  if (self->key == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate buffer to the heap");
    exit(EXIT_FAILURE);
  }

  memcpy(key, self->key, self->keylen);

  *size = self->keylen;

  return key;
}

static int set_bucket_haskey(set_bucket_t *self, const void *key, const size_t keylen)
{
  return self->keylen == keylen && memcmp(self->key, key, keylen) == 0;
}

set_t *set_new(const size_t size)
{
  set_t *self = NULL;

  self = (set_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate set to the heap");
    exit(EXIT_FAILURE);
  }

  self->buckets = (set_bucket_t **)calloc(size, sizeof(*self->buckets));
  if (self->buckets == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "could not allocate set.buckets to the heap");
    exit(EXIT_FAILURE);
  }

  self->size = size;

  return self;
}

void set_destroy(set_t *self)
{
  if (self != NULL)
  {
    if (self->buckets != NULL)
    {
      uint64_t i;

      for (i = 0UL; i < self->size; i++)
      {
        if (self->buckets[i] == NULL)
        {
          continue;
        }

        set_bucket_destroy(&self->buckets[i]);
      }

      free(self->buckets);
      self->buckets = NULL;
    }

    free(self);
    self = NULL;
  }
}

#define SEED 2

void *set_getall(set_t *self, size_t *overall_size)
{
  void *data = NULL;
  void *key = NULL;
  size_t size = 0UL;
  uint64_t i;

  *overall_size = 0UL;

  for (i = 0UL; i < self->size; i++)
  {
    if (self->buckets[i] == NULL)
    {
      continue;
    }

    key = set_bucket_key(self->buckets[i], &size);

    if (data == NULL)
    {
      data = malloc(size * sizeof(uint8_t));
      if (data == NULL)
      {
        fprintf(stderr, "%s(): %s\n", __func__, "could not allocate buffer to the heap");
        exit(EXIT_FAILURE);
      }
      *overall_size += size;
    }
    else
    {
      void *old = NULL;
      old = data;
      data = NULL;

      *overall_size += size;
      data = realloc(old, *overall_size * sizeof(uint8_t));
      if (data == NULL)
      {
        fprintf(stderr, "%s(): %s\n", __func__, "could not reallocate buffer to the heap");
        exit(EXIT_FAILURE);
      }
    }

    memcpy((data + (*overall_size - size)), key, size);

    if (key != NULL)
    {
      free(key);
      key = NULL;
    }
  }

  return data;
}

int set_exists(set_t *self, const void *key, const size_t keylen)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] == NULL)
    {
      return 0;
    }

    if (0 == set_bucket_haskey(self->buckets[j], key, keylen))
    {
      continue;
    }

    return 1;
  }

  return 0;
}

int set_add(set_t *self, const void *key, const size_t keylen)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] != NULL)
    {
      if (1 == set_bucket_haskey(self->buckets[j], key, keylen))
      {
        return 0;
      }

      continue;
    }

    self->buckets[j] = set_bucket_new(key, keylen);
    return 0;
  }

  return (-1);
}

int set_remove(set_t *self, const void *key, const size_t keylen)
{
  const uint64_t key_hashed = __hash__(key, keylen, SEED);
  uint64_t i;
  uint64_t j;

  for (i = 0UL; i < self->size; i++)
  {
    j = (key_hashed + i) % self->size;

    if (self->buckets[j] == NULL)
    {
      return (-1);
    }

    if (0 == set_bucket_haskey(self->buckets[j], key, keylen))
    {
      continue;
    }

    set_bucket_destroy(&self->buckets[j]);
    return 0;
  }

  return (-1);
}

//#include "map.h"
//#include "set.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct graph_node
{
  void *data;
  size_t size;
  set_t *edges;
};

typedef struct graph_node graph_node_t;

graph_node_t *graph_node_create(const void *data, const size_t size, const size_t max_edges)
{
  graph_node_t *self = NULL;

  self = (graph_node_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate graph node to the heap");
    exit(EXIT_FAILURE);
  }

  self->data = data;
  self->size = size;
  self->edges = set_new(max_edges);

  return self;
}

void graph_node_destroy(graph_node_t *self)
{
  if (self != NULL)
  {
    if (self->edges != NULL)
    {
      set_destroy(self->edges);
      self->edges = NULL;
    }

    free(self);
    self = NULL;
  }
}

typedef struct graph_edge graph_edge_t;

void graph_node_add_edge(graph_node_t *self, const graph_edge_t *edge)
{
  set_add(self->edges, &edge, sizeof(*edge));
}

struct graph_edge
{
  graph_node_t *dest;
};

graph_edge_t *graph_edge_create(const graph_node_t *dest)
{
  graph_edge_t *self = NULL;

  self = (graph_edge_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate graph edge to the heap");
    exit(EXIT_FAILURE);
  }

  self->dest = (graph_node_t *)dest;

  return self;
}

void graph_edge_destroy(graph_edge_t *self)
{
  if (self != NULL)
  {
    free(self);
    self = NULL;
  }
}

struct graph
{
  map_t *nodes;
};

typedef struct graph graph_t;

graph_t *graph_create(const size_t max_nodes)
{
  graph_t *self = NULL;

  self = (graph_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate graph to the heap");
    exit(EXIT_FAILURE);
  }

  self->nodes = map_new(max_nodes);

  return self;
}

void graph_destroy(graph_t *self)
{
  if (self != NULL)
  {
    if (self->nodes != NULL)
    {
      free(self->nodes);
      self->nodes = NULL;
    }

    free(self);
    self = NULL;
  }
}

graph_node_t *graph_add_node(graph_t *self, const void *data, const size_t size)
{
  graph_node_t *node = NULL;

  if (NULL == (node = map_get(self->nodes, data, size)))
  {
    node = graph_node_create(data, size, 16);
    map_set(self->nodes, data, size, node, sizeof(node));
  }

  return node;
}

void graph_add_edge(graph_t *self, const void *a, const size_t as, const void *b, const size_t bs)
{
  graph_node_t *src = graph_add_node(self->nodes, a, as);
  graph_node_t *dst = graph_add_node(self->nodes, b, bs);

  graph_edge_t *edge = graph_edge_create(dst);
  graph_node_add_edge(src, edge);
}

int main(void)
{
  graph_t *g = NULL;
  g = graph_create(16);
  graph_destroy(g);
  return 0;
}






