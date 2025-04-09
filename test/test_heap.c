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
#include "heap.h"

static int int_compare(const void *a, const void *b)
{
  const int ai = *(const int *)a;
  const int bi = *(const int *)b;

  if (ai < bi)
  {
    return (-1);
  }
  else if (ai > bi)
  {
    return 1;
  }

  else return 0;
}

static void test_heap_empty(void **state)
{
  UNUSED(state);

  heap_t *heap = NULL;
  heap = heap_new(int_compare);
  assert_non_null(heap);

  assert_null(heap_poll(heap));
  assert_null(heap_pop(heap));

  heap_destroy(heap);
}

static void test_heap_single(void **state)
{
  UNUSED(state);

  heap_t *heap = NULL;
  heap = heap_new(int_compare);
  assert_non_null(heap);
  
  int value = 42;
  int ret = heap_push(heap, &value);
  assert_int_equal(ret, 0);

  void *polled = heap_poll(heap);
  assert_non_null(polled);
  assert_int_equal(*(int *)polled, 42);

  void *popped = heap_pop(heap);
  assert_non_null(popped);
  assert_int_equal(*(int *)popped, 42);
  assert_null(heap_poll(heap));

  heap_destroy(heap);
}

static void test_heap_order(void **state)
{
  UNUSED(state);

  heap_t *heap = heap_new(int_compare);
  assert_non_null(heap);

  int values[] = {10, 5, 7, 2, 15, 3};
  size_t count = sizeof(values)/sizeof(values[0]);

  for (size_t i = 0; i < count; i++)
  {
    int ret = heap_push(heap, &values[i]);
    assert_int_equal(ret, 0);
  }

  int expected[] = {2, 3, 5, 7, 10, 15};
  for (size_t i = 0; i < count; i++)
  {
    void *pop_val = heap_pop(heap);
    assert_non_null(pop_val);
    int out = *(int *)pop_val;
    assert_int_equal(out, expected[i]);
  }

  assert_null(heap_poll(heap));
  heap_destroy(heap);
}

static void test_heap_merge(void **state)
{
  UNUSED(state);

  heap_t *heap1 = heap_new(int_compare);
  assert_non_null(heap1);
  int a = 1, b = 5, c = 9;
  assert_int_equal(heap_push(heap1, &a), 0);
  assert_int_equal(heap_push(heap1, &b), 0);
  assert_int_equal(heap_push(heap1, &c), 0);

  heap_t *heap2 = heap_new(int_compare);
  assert_non_null(heap2);
  int d = 2, e = 6, f = 7;
  assert_int_equal(heap_push(heap2, &d), 0);
  assert_int_equal(heap_push(heap2, &e), 0);
  assert_int_equal(heap_push(heap2, &f), 0);

  heap_t *merged = heap_merge(heap1, heap2);
  assert_non_null(merged);

  int expected[] = {1, 2, 5, 6, 7, 9};
  size_t expected_count = sizeof(expected) / sizeof(expected[0]);
  for (size_t i = 0; i < expected_count; i++)
  {
    void *pop_val = heap_pop(merged);
    assert_non_null(pop_val);
    int out = *(int *)pop_val;
    assert_int_equal(out, expected[i]);
  }

  assert_null(heap_poll(merged));
  heap_destroy(merged);
}

static void test_heap_poll_consistency(void **state)
{
  UNUSED(state);

  heap_t *heap = heap_new(int_compare);
  assert_non_null(heap);

  int value1 = 42, value2 = 10;
  assert_int_equal(heap_push(heap, &value1), 0);
  void *p1 = heap_poll(heap);
  assert_non_null(p1);
  assert_int_equal(*(int *)p1, 42);

  assert_int_equal(heap_push(heap, &value2), 0);
  void *p2 = heap_poll(heap);
  assert_non_null(p2);
  assert_int_equal(*(int *)p2, 10);

  void *p3 = heap_poll(heap);
  assert_non_null(p3);
  assert_int_equal(*(int *)p3, 10);
  heap_destroy(heap);
}

static void test_heap_merge_null(void **state)
{
  UNUSED(state);

  heap_t *heap1 = heap_new(int_compare);
  assert_non_null(heap1);
  int value = 42;
  assert_int_equal(heap_push(heap1, &value), 0);

  heap_t *merged = heap_merge(heap1, NULL);
  assert_non_null(merged);
  void *p = heap_poll(merged);
  assert_non_null(p);
  assert_int_equal(*(int *)p, 42);
  heap_destroy(merged);

  heap_t *heap2 = heap_new(int_compare);
  assert_non_null(heap2);
  int value2 = 100;
  assert_int_equal(heap_push(heap2, &value2), 0);

  merged = heap_merge(NULL, heap2);
  assert_non_null(merged);
  p = heap_poll(merged);
  assert_non_null(p);
  assert_int_equal(*(int *)p, 100);
  heap_destroy(merged);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_heap_empty),
    cmocka_unit_test(test_heap_single),
    cmocka_unit_test(test_heap_order),
    cmocka_unit_test(test_heap_merge),
    cmocka_unit_test(test_heap_poll_consistency),
    cmocka_unit_test(test_heap_merge_null),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
