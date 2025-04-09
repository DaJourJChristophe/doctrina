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
#include "trie.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHILDREN 255

struct trie_node
{
  bool eow;
  struct trie_node *children[NUM_CHILDREN];
};

typedef struct trie_node trie_node_t;

trie_node_t *trie_node_new(void)
{
  trie_node_t *self = NULL;

  self = (trie_node_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate trie node to the heap");
    exit(EXIT_FAILURE);
  }

  return self;
}

void trie_node_destroy(trie_node_t *self)
{
  if (self != NULL)
  {
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
      if (self->children[i] == NULL)
      {
        continue;
      }

      trie_node_destroy(self->children[i]);
      self->children[i] = NULL;
    }

    free(self);
    self = NULL;
  }
}

struct trie
{
  trie_node_t root;
};

trie_t *trie_new(void)
{
  trie_t *self = NULL;

  self = (trie_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate trie to the heap");
    exit(EXIT_FAILURE);
  }

  return self;
}

void trie_destroy(trie_t *self)
{
  if (self != NULL)
  {
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
      if (self->root.children[i] == NULL)
      {
        continue;
      }

      trie_node_destroy(self->root.children[i]);
      self->root.children[i] = NULL;
    }

    free(self);
    self = NULL;
  }
}

void trie_insert(trie_t *self, const uint8_t *key)
{
  trie_node_t *node = &self->root;

  for (; *key != '\0'; key++)
  {
    const int index = *key;

    if (node->children[index] == NULL)
    {
      node->children[index] = trie_node_new();
    }

    node = node->children[index];
  }

  node->eow = true;
}

bool trie_search(trie_t *self, const uint8_t *key)
{
  trie_node_t *node = &self->root;

  for (; *key != '\0'; key++)
  {
    const int index = *key;

    if (node->children[index] == NULL)
    {
      return false;
    }

    node = node->children[index];
  }

  return node->eow;
}
