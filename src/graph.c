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
#include "graph.h"
#include "map.h"
#include "set.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct graph_edge
{
  graph_node_t *dest;
};

typedef struct graph_edge graph_edge_t;

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
    self->dest = NULL;
    free(self);
    self = NULL;
  }
}

struct graph_node
{
  void *data;
  size_t size;
  set_t *edges;
};

graph_node_t *graph_node_create(const void *data, const size_t size, const size_t max_edges)
{
  graph_node_t *self = NULL;

  self = (graph_node_t *)calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate graph node to the heap");
    exit(EXIT_FAILURE);
  }

  self->data = (void *)data;
  self->size = size;
  self->edges = set_new(max_edges);

  return self;
}

void graph_node_destroy(graph_node_t *self)
{
  if (self != NULL)
  {
    graph_edge_t *edge = NULL;
    void *tmp = NULL;
    size_t size = 0UL;

    for (uint64_t i = 0UL; i < self->edges->size; i++)
    {
      if (self->edges->buckets[i] == NULL)
      {
        continue;
      }

      tmp = set_bucket_key(self->edges->buckets[i], &size);
      if (tmp == NULL)
      {
        continue;
      }

      edge = *(graph_edge_t **)tmp;
      free(tmp);

      if (edge == NULL)
      {
        continue;
      }

      graph_edge_destroy(edge);
    }

    set_destroy(self->edges);
    self->edges = NULL;

    free(self);
    self = NULL;
  }
}

void graph_node_add_edge(graph_node_t *self, const graph_edge_t *edge)
{
  if (0 > set_add(self->edges, &edge, sizeof(edge)))
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not add edge to the set");
    exit(EXIT_FAILURE);
  }
}

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
    graph_node_t *node = NULL;
    void *tmp = NULL;

    for (uint64_t i = 0UL; i < self->nodes->size; i++)
    {
      if (self->nodes->buckets[i] == NULL)
      {
        continue;
      }

      tmp = bucket_data(self->nodes->buckets[i]);
      if (tmp == NULL)
      {
        continue;
      }

      node = *(graph_node_t **)tmp;
      free(tmp);

      if (node == NULL)
      {
        continue;
      }

      graph_node_destroy(node);
    }

    map_destroy(self->nodes);
    self->nodes = NULL;

    free(self);
    self = NULL;
  }
}

static graph_node_t *graph_add_node(graph_t *self, const void *data, const size_t size)
{
  graph_node_t *node = NULL;
  size_t data_size = 0UL;

  const uintptr_t *addr = map_get(self->nodes, data, size, &data_size);

  if (addr == NULL)
  {
    node = graph_node_create(data, size, 16);

    if (0 > map_set(self->nodes, data, size, &node, sizeof(node)))
    {
      fprintf(stderr, "%s(): %s\n", __func__, "Could not insert key-value pair into map");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    node = (graph_node_t *)*addr;
    free((void *)addr);
  }

  return node;
}

void graph_add_edge(graph_t *self, const void *a, const size_t as, const void *b, const size_t bs)
{
  const graph_node_t *dst = graph_add_node(self, b, bs);
  const graph_edge_t *edge = graph_edge_create(dst);
  graph_node_t *src = graph_add_node(self, a, as);
  graph_node_add_edge(src, edge);
}

// int main(void)
// {
//   graph_t *g = NULL;
//   g = graph_create(16);
//   graph_add_edge(g, &(uint32_t){5}, sizeof(uint32_t), &(uint32_t){6}, sizeof(uint32_t));
//   graph_add_edge(g, &(uint32_t){5}, sizeof(uint32_t), &(uint32_t){7}, sizeof(uint32_t));
//   graph_destroy(g);
//   return 0;
// }
