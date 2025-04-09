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
#include "stack.h"

#include <stdlib.h>

static void test_stack_create_destroy(void **state)
{
  UNUSED(state);

  const size_t capacity = 64;
  stack_t *s = NULL;

  s = stack_create(capacity);
  assert_non_null(s);
  assert_int_equal(s->top, 0);
  assert_int_equal(s->cap, capacity);

  stack_destroy(s);
}

static void test_empty_stack_peek_pop(void **state)
{
  UNUSED(state);

  stack_t *s = NULL;

  s = stack_create(64);
  assert_non_null(s);

  void *peek_result = stack_peek(s, sizeof(int));
  assert_null(peek_result);

  void *pop_result = stack_pop(s, sizeof(int));
  assert_null(pop_result);

  stack_destroy(s);
}

static void test_stack_push_pop_int(void **state)
{
  UNUSED(state);

  stack_t *s = NULL;

  s = stack_create(64);
  assert_non_null(s);

  int value = 42;
  int ret = stack_push(s, &value, sizeof(value));
  assert_int_equal(ret, 0);

  int *peek_value = (int *)stack_peek(s, sizeof(value));
  assert_non_null(peek_value);
  assert_int_equal(*peek_value, value);
  free(peek_value);

  int *pop_value = (int *)stack_pop(s, sizeof(value));
  assert_non_null(pop_value);
  assert_int_equal(*pop_value, value);
  free(pop_value);

  assert_null(stack_peek(s, sizeof(value)));
  assert_null(stack_pop(s, sizeof(value)));

  stack_destroy(s);
}

static void test_stack_push_pop_multiple(void **state)
{
  UNUSED(state);

  stack_t *s = NULL;

  s = stack_create(128);
  assert_non_null(s);

  int val1 = 100;
  int val2 = 200;

  int ret = stack_push(s, &val1, sizeof(val1));
  assert_int_equal(ret, 0);

  ret = stack_push(s, &val2, sizeof(val2));
  assert_int_equal(ret, 0);

  int *pop_val2 = (int *)stack_pop(s, sizeof(val2));
  assert_non_null(pop_val2);
  assert_int_equal(*pop_val2, val2);
  free(pop_val2);

  int *pop_val1 = (int *)stack_pop(s, sizeof(val1));
  assert_non_null(pop_val1);
  assert_int_equal(*pop_val1, val1);
  free(pop_val1);

  stack_destroy(s);
}

static void test_stack_overflow(void **state)
{
  UNUSED(state);

  stack_t *s = NULL;

  s = stack_create(10);
  assert_non_null(s);

  int a = 1234;
  int ret = stack_push(s, &a, sizeof(a));
  assert_int_equal(ret, 0);

  int b = 5678;
  ret = stack_push(s, &b, sizeof(b));
  assert_int_equal(ret, 0);

  char extra[2] = { 'A', 'B' };
  ret = stack_push(s, extra, sizeof(extra));
  assert_int_equal(ret, -1);

  stack_destroy(s);
}

static void test_stack_push_peek_no_modification(void **state)
{
  UNUSED(state);

  stack_t *s = stack_create(64);
  assert_non_null(s);

  int value = 777;
  int ret = stack_push(s, &value, sizeof(value));
  assert_int_equal(ret, 0);

  int *peek1 = (int *)stack_peek(s, sizeof(value));
  assert_non_null(peek1);
  assert_int_equal(*peek1, value);

  int *peek2 = (int *)stack_peek(s, sizeof(value));
  assert_non_null(peek2);
  assert_int_equal(*peek2, value);

  free(peek1);
  free(peek2);

  int *pop_val = (int *)stack_pop(s, sizeof(value));
  assert_non_null(pop_val);
  assert_int_equal(*pop_val, value);
  free(pop_val);

  stack_destroy(s);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_stack_create_destroy),
    cmocka_unit_test(test_empty_stack_peek_pop),
    cmocka_unit_test(test_stack_push_pop_int),
    cmocka_unit_test(test_stack_push_pop_multiple),
    cmocka_unit_test(test_stack_overflow),
    cmocka_unit_test(test_stack_push_peek_no_modification),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
