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
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "heap.h"

typedef int (*pq_compare_t)(const void *a, const void *b);

typedef struct priority_queue priority_queue_t;

priority_queue_t *pq_new(pq_compare_t cmp);

void pq_destroy(priority_queue_t *pq);

int pq_push(priority_queue_t *pq, const void *data);

void *pq_peek(priority_queue_t *pq);

void *pq_pop(priority_queue_t *pq);

priority_queue_t *pq_merge(priority_queue_t *pq1, priority_queue_t *pq2);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*PRIORITY_QUEUE_H*/
