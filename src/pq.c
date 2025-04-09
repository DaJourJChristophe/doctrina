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
#include "pq.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct priority_queue
{
  heap_t *heap;
};

priority_queue_t *pq_new(pq_compare_t cmp)
{
  priority_queue_t *pq = NULL;

  pq = (priority_queue_t *)calloc(1UL, sizeof(*pq));
  if (pq == NULL)
  {
    fprintf(stderr, "pq_new(): could not allocate memory for priority queue\n");
    exit(EXIT_FAILURE);
  }

  pq->heap = heap_new(cmp);
  return pq;
}

void pq_destroy(priority_queue_t *pq)
{
  if (pq != NULL)
  {
    heap_destroy(pq->heap);
    free(pq);
    pq = NULL;
  }
}

int pq_push(priority_queue_t *pq, const void *data)
{
  if (pq == NULL)
  {
    return (-1);
  }

  return heap_push(pq->heap, data);
}

void *pq_peek(priority_queue_t *pq)
{
  if (pq == NULL)
  {
    return NULL;
  }

  return heap_poll(pq->heap);
}

void *pq_pop(priority_queue_t *pq)
{
  if (pq == NULL)
  {
    return NULL;
  }

  return heap_pop(pq->heap);
}

priority_queue_t *pq_merge(priority_queue_t *pq1, priority_queue_t *pq2)
{
  if (pq1 == NULL)
  {
    return pq2;
  }

  if (pq2 == NULL)
  {
    return pq1;
  }

  pq1->heap = heap_merge(pq1->heap, pq2->heap);
  free(pq2);

  return pq1;
}
