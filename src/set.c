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
#include "set.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bucket
{
  size_t   keylen;
  uint8_t *key;
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

static bucket_t *bucket_new(const void *key, const size_t keylen)
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

    free(*self);
    *self = NULL;
  }
}

static void *bucket_key(bucket_t *self, size_t *size)
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

static int bucket_haskey(bucket_t *self, const void *key, const size_t keylen)
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

  self->buckets = (bucket_t **)calloc(size, sizeof(*self->buckets));
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

    key = bucket_key(self->buckets[i], &size);

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

    if (0 == bucket_haskey(self->buckets[j], key, keylen))
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
      if (1 == bucket_haskey(self->buckets[j], key, keylen))
      {
        return 0;
      }

      continue;
    }

    self->buckets[j] = bucket_new(key, keylen);
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

    if (0 == bucket_haskey(self->buckets[j], key, keylen))
    {
      continue;
    }

    bucket_destroy(&self->buckets[j]);
    return 0;
  }

  return (-1);
}
