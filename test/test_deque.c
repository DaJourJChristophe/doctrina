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
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>

#include "cmocka.h"

#include "common.h"
#include "deque.h"

#include <stdio.h>
#include <stdlib.h>

#define RING_BUFFER_CAPACITY 32UL

static void ring_buffer_new_test(void unused **state)
{
  ring_buffer_t *buffer = NULL;
  void *empty = NULL;

  buffer = ring_buffer_create(RING_BUFFER_CAPACITY);
  assert_non_null(buffer);
  assert_int_equal(ring_buffer_get_cap(buffer), RING_BUFFER_CAPACITY);

  assert_int_equal(ring_buffer_reader_get_head(ring_buffer_get_reader(buffer)), 0UL);
  assert_int_equal(ring_buffer_reader_get_tail(ring_buffer_get_reader(buffer)), 0UL);
  assert_int_equal(ring_buffer_writer_get_head(ring_buffer_get_writer(buffer)), 0UL);
  assert_int_equal(ring_buffer_writer_get_tail(ring_buffer_get_writer(buffer)), RING_BUFFER_CAPACITY);

  empty = calloc(32UL, sizeof(uint8_t));
  if (empty == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate empty block to the heap");
    exit(EXIT_FAILURE);
  }

  assert_memory_equal(ring_buffer_get_data(buffer), empty, RING_BUFFER_CAPACITY);

  if (empty != NULL)
  {
    free(empty);
    empty = NULL;
  }

  ring_buffer_destroy(buffer);
}

static void ring_buffer_enqueue_test(void unused **state)
{
  ring_buffer_t *buffer = NULL;

  buffer = ring_buffer_create(RING_BUFFER_CAPACITY);

  assert_int_equal(0, ring_buffer_enqueue(buffer, &(uint32_t){55U}, sizeof(uint32_t)));
  assert_int_equal(sizeof(uint32_t), ring_buffer_size(buffer));

  assert_int_equal(0, ring_buffer_enqueue(buffer, &(uint32_t){56U}, sizeof(uint32_t)));
  assert_int_equal((2 * sizeof(uint32_t)), ring_buffer_size(buffer));

  assert_int_equal(0, ring_buffer_enqueue(buffer, &(uint32_t){57U}, sizeof(uint32_t)));
  assert_int_equal((3 * sizeof(uint32_t)), ring_buffer_size(buffer));

  assert_int_equal(0, ring_buffer_enqueue(buffer, &(uint32_t){58U}, sizeof(uint32_t)));
  assert_int_equal((4 * sizeof(uint32_t)), ring_buffer_size(buffer));

  const uint8_t *recent_data = ring_buffer_get_data(buffer);

  assert_memory_equal(&(uint32_t){55U}, (uint32_t *)recent_data, sizeof(uint32_t));
  assert_memory_equal(&(uint32_t){56U}, (uint32_t *)(recent_data + sizeof(uint32_t)), sizeof(uint32_t));
  assert_memory_equal(&(uint32_t){57U}, (uint32_t *)(recent_data + (2 * sizeof(uint32_t))), sizeof(uint32_t));
  assert_memory_equal(&(uint32_t){58U}, (uint32_t *)(recent_data + (3 * sizeof(uint32_t))), sizeof(uint32_t));

  ring_buffer_destroy(buffer);
}

static void ring_buffer_dequeue_test(void unused **state)
{
  ring_buffer_t *buffer = NULL;
  uint32_t *item = NULL;

  buffer = ring_buffer_create(RING_BUFFER_CAPACITY);

  ring_buffer_enqueue(buffer, &(uint32_t){55U}, sizeof(uint32_t));
  ring_buffer_enqueue(buffer, &(uint32_t){56U}, sizeof(uint32_t));
  ring_buffer_enqueue(buffer, &(uint32_t){57U}, sizeof(uint32_t));
  ring_buffer_enqueue(buffer, &(uint32_t){58U}, sizeof(uint32_t));

  assert_int_equal((4 * sizeof(uint32_t)), ring_buffer_size(buffer));
  item = ring_buffer_dequeue(buffer, sizeof(uint32_t));
  assert_non_null(item);
  assert_memory_equal(&(uint32_t){55U}, item, sizeof(*item));
  if (item != NULL)
  {
    free(item);
    item = NULL;
  }

  assert_int_equal((3 * sizeof(uint32_t)), ring_buffer_size(buffer));
  item = ring_buffer_dequeue(buffer, sizeof(uint32_t));
  assert_non_null(item);
  assert_memory_equal(&(uint32_t){56U}, item, sizeof(*item));
  if (item != NULL)
  {
    free(item);
    item = NULL;
  }

  assert_int_equal((2 * sizeof(uint32_t)), ring_buffer_size(buffer));
  item = ring_buffer_dequeue(buffer, sizeof(uint32_t));
  assert_non_null(item);
  assert_memory_equal(&(uint32_t){57U}, item, sizeof(*item));
  if (item != NULL)
  {
    free(item);
    item = NULL;
  }

  assert_int_equal(sizeof(uint32_t), ring_buffer_size(buffer));
  item = ring_buffer_dequeue(buffer, sizeof(uint32_t));
  assert_non_null(item);
  assert_memory_equal(&(uint32_t){58U}, item, sizeof(*item));
  if (item != NULL)
  {
    free(item);
    item = NULL;
  }

  assert_int_equal(0UL, ring_buffer_size(buffer));
  item = ring_buffer_dequeue(buffer, sizeof(uint32_t));
  assert_null(item);

  ring_buffer_destroy(buffer);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(ring_buffer_new_test),
    cmocka_unit_test(ring_buffer_enqueue_test),
    cmocka_unit_test(ring_buffer_dequeue_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
