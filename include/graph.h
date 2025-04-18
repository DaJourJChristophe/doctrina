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
#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "map.h"

#include <stddef.h>

typedef struct graph_node graph_node_t;

struct graph
{
  map_t *nodes;
};

typedef struct graph graph_t;

graph_t *graph_create(const size_t max_nodes);

void graph_destroy(graph_t *self);

void graph_bfs(graph_t *self, const void *start_data, const size_t size);

void graph_dfs(graph_t *self, const void *start_data, const size_t size);

static graph_node_t *graph_add_node(graph_t *self, const void *data, const size_t size);

void graph_add_edge(graph_t *self, const void *a, const size_t as, const void *b, const size_t bs);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*GRAPH_H*/
