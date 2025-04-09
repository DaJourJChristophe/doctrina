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
#include "heap.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct heap_node
{
  void *data;
  struct heap_node *child;
  struct heap_node *sibling;
};

typedef struct heap_node heap_node_t;

struct heap
{
  heap_node_t *root;
  heap_compare_t cmp;
};

static heap_node_t *merge_nodes(heap_node_t *node1, heap_node_t *node2, heap_compare_t cmp)
{
  if (node1 == NULL)
  {
    return node2;
  }

  if (node2 == NULL)
  {
    return node1;
  }

  if (cmp(node1->data, node2->data) <= 0)
  {
    node2->sibling = node1->child;
    node1->child = node2;
    return node1;
  }

  node1->sibling = node2->child;
  node2->child = node1;
  return node2;
}

static heap_node_t *merge_pairs(heap_node_t *first, heap_compare_t cmp)
{
  heap_node_t *result = NULL;

  if (first == NULL || first->sibling == NULL)
  {
    return first;
  }

  heap_node_t *second = first->sibling;
  heap_node_t *remaining = second->sibling;

  first->sibling = NULL;
  second->sibling = NULL;

  result = merge_nodes(first, second, cmp);
  result = merge_nodes(result, merge_pairs(remaining, cmp), cmp);

  return result;
}

static void destroy_node(heap_node_t *node)
{
  if (node == NULL)
  {
    return;
  }

  destroy_node(node->child);
  node->child = NULL;

  destroy_node(node->sibling);
  node->sibling = NULL;

  free(node);
  node = NULL;
}


heap_t *heap_new(heap_compare_t cmp)
{
  heap_t *heap = NULL;

  heap = (heap_t *)calloc(1UL, sizeof(*heap));
  if (heap == NULL)
  {
    fprintf(stderr, "heap_new(): could not allocate pairing heap\n");
    exit(EXIT_FAILURE);
  }

  heap->cmp = cmp;

  return heap;
}

void heap_destroy(heap_t *heap)
{
  if (heap != NULL)
  {
    if (heap != NULL)
    {
      destroy_node(heap->root);
      heap->root = NULL;
    }

    free(heap);
    heap = NULL;
  }
}

int heap_push(heap_t *heap, const void *data)
{
  heap_node_t *node = NULL;

  node = (heap_node_t *)calloc(1UL, sizeof(*node));
  if (node == NULL)
  {
    fprintf(stderr, "heap_push(): could not allocate node\n");
    exit(EXIT_FAILURE);
  }

  node->data = (void *)data;
  heap->root = merge_nodes(heap->root, node, heap->cmp);

  return 0;
}

void *heap_poll(heap_t *heap)
{
  if (heap == NULL || heap->root == NULL)
  {
    return NULL;
  }

  return heap->root->data;
}

void *heap_pop(heap_t *heap)
{
  heap_node_t *old_root = NULL;
  void *min_data = NULL;

  if (heap == NULL || heap->root == NULL)
  {
    return NULL;
  }

  old_root = heap->root;
  min_data = old_root->data;

  heap->root = merge_pairs(old_root->child, heap->cmp);
  free(old_root);

  return min_data;
}

heap_t *heap_merge(heap_t *h1, heap_t *h2)
{
  if (h1 == NULL)
  {
    return h2;
  }

  if (h2 == NULL)
  {
    return h1;
  }

  h1->root = merge_nodes(h1->root, h2->root, h1->cmp);
  free(h2);

  return h1;
}
