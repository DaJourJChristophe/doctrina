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
#include "set.h"

#include <stdlib.h>
#include <string.h>

static int contains_subarray(const uint8_t *haystack, size_t haystack_len, const uint8_t *needle, size_t needle_len)
{
  size_t i;

  if (needle_len == 0 || haystack_len < needle_len)
  {
    return 0;
  }

  for (i = 0; i <= haystack_len - needle_len; i++)
  {
    if (memcmp(haystack + i, needle, needle_len) == 0)
    {
      return 1;
    }
  }

  return 0;
}

static void test_set_create_destroy(void **state)
{
  UNUSED(state);
  set_t *s = NULL;
  s = set_new(10);
  assert_non_null(s);
  set_destroy(s);
}

static void test_set_add_exists(void **state)
{
  UNUSED(state);

  set_t *s = NULL;
  s = set_new(10);
  assert_non_null(s);

  const char *key = "test";
  int ret = set_add(s, key, strlen(key) + 1);
  assert_int_equal(ret, 0);

  assert_int_equal(set_exists(s, key, strlen(key) + 1), 1);

  const char *key2 = "foo";
  assert_int_equal(set_exists(s, key2, strlen(key2) + 1), 0);

  set_destroy(s);
}

static void test_set_duplicate(void **state)
{
  UNUSED(state);

  set_t *s = NULL;
  s = set_new(10);
  assert_non_null(s);

  const char *key = "duplicate";
  int ret = set_add(s, key, strlen(key) + 1);
  assert_int_equal(ret, 0);

  ret = set_add(s, key, strlen(key) + 1);
  assert_int_equal(ret, 0);

  size_t overall_size = 0;
  uint8_t *all_keys = set_getall(s, &overall_size);
  assert_non_null(all_keys);
  assert_int_equal(overall_size, strlen(key) + 1);

  free(all_keys);
  set_destroy(s);
}

static void test_set_remove(void **state)
{
  UNUSED(state);

  set_t *s = NULL;
  s = set_new(10);
  assert_non_null(s);

  const char *key = "remove_me";
  int ret = set_add(s, key, strlen(key) + 1);
  assert_int_equal(ret, 0);

  assert_int_equal(set_exists(s, key, strlen(key) + 1), 1);

  ret = set_remove(s, key, strlen(key) + 1);
  assert_int_equal(ret, 0);

  assert_int_equal(set_exists(s, key, strlen(key) + 1), 0);
  set_destroy(s);
}

static void test_set_remove_nonexistent(void **state)
{
  UNUSED(state);

  set_t *s = NULL;
  s = set_new(10);
  assert_non_null(s);

  const char *key = "nonexistent";
  int ret = set_remove(s, key, strlen(key) + 1);
  assert_int_equal(ret, -1);

  set_destroy(s);
}

static void test_set_getall(void **state)
{
  UNUSED(state);

  set_t *s = set_new(10);
  assert_non_null(s);

  const char *keys[] = {"alpha", "beta", "gamma", "delta"};
  size_t num_keys = sizeof(keys) / sizeof(keys[0]);
  size_t total_expected = 0;

  for (size_t i = 0; i < num_keys; i++)
  {
    int ret = set_add(s, keys[i], strlen(keys[i]) + 1);
    assert_int_equal(ret, 0);
    total_expected += (strlen(keys[i]) + 1);
  }

  size_t overall_size = 0;
  uint8_t *all_keys = set_getall(s, &overall_size);
  assert_non_null(all_keys);
  assert_int_equal(overall_size, total_expected);

  for (size_t i = 0; i < num_keys; i++)
  {
    const uint8_t *needle = (const uint8_t *)keys[i];
    size_t needle_len = strlen(keys[i]) + 1;
    int found = contains_subarray(all_keys, overall_size, needle, needle_len);
    assert_true(found);
  }

  free(all_keys);
  set_destroy(s);
}

static void test_set_overflow(void **state)
{
  UNUSED(state);

  set_t *s = set_new(3);
  assert_non_null(s);

  const char *keys[] = {"a", "b", "c", "d"};
  int ret = set_add(s, keys[0], strlen(keys[0]) + 1);
  assert_int_equal(ret, 0);
  ret = set_add(s, keys[1], strlen(keys[1]) + 1);
  assert_int_equal(ret, 0);
  ret = set_add(s, keys[2], strlen(keys[2]) + 1);
  assert_int_equal(ret, 0);

  ret = set_add(s, keys[3], strlen(keys[3]) + 1);
  assert_int_equal(ret, -1);

  set_destroy(s);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_set_create_destroy),
    cmocka_unit_test(test_set_add_exists),
    cmocka_unit_test(test_set_duplicate),
    cmocka_unit_test(test_set_remove),
    cmocka_unit_test(test_set_remove_nonexistent),
    cmocka_unit_test(test_set_getall),
    cmocka_unit_test(test_set_overflow),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
