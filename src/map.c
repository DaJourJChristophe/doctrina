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
#include "internal/hash.h"
#include "common.h"
#include "map.h"

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

    if (size != NULL)
    {
      *size = bucket_size(self->buckets[j]);
    }

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
