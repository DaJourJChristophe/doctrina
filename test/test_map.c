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
#include "map.h"

#include <stdlib.h>
#include <string.h>

static void test_map_create_destroy(void **state)
{
  UNUSED(state);

  size_t capacity = 10;
  map_t *m = map_new(capacity);

  assert_non_null(m);
  assert_int_equal(m->size, capacity);

  map_destroy(m);
}

static void test_map_set_get_exists(void **state)
{
  UNUSED(state);

  map_t *m = map_new(10);
  assert_non_null(m);

  const char *key = "foo";
  const char *value = "bar";

  int ret = map_set(m, key, strlen(key), value, strlen(value) + 1);
  assert_int_equal(ret, 0);

  assert_int_equal(map_exists(m, key, strlen(key)), 1);

  size_t data_size = 0;
  void *data = map_get(m, key, strlen(key), &data_size);
  assert_non_null(data);
  assert_int_equal(data_size, strlen(value) + 1);
  assert_string_equal((char *)data, value);
  free(data);

  map_destroy(m);
}

static void test_map_update(void **state)
{
  UNUSED(state);

  map_t *m = map_new(10);
  assert_non_null(m);

  const char *key = "update_key";
  const char *init_val = "oldVal";
  const char *upd_val  = "newVal";

  int ret = map_set(m, key, strlen(key), init_val, strlen(init_val) + 1);
  assert_int_equal(ret, 0);

  ret = map_set(m, key, strlen(key), upd_val, strlen(upd_val) + 1);
  assert_int_equal(ret, 0);

  size_t data_size = 0;
  void *data = map_get(m, key, strlen(key), &data_size);
  assert_non_null(data);
  assert_int_equal(data_size, strlen(upd_val) + 1);
  assert_string_equal((char *)data, upd_val);
  free(data);

  map_destroy(m);
}

static void test_map_delete(void **state)
{
  UNUSED(state);

  map_t *m = map_new(10);
  assert_non_null(m);

  const char *key = "delete_key";
  const char *value = "some_data";
  int ret = map_set(m, key, strlen(key), value, strlen(value) + 1);
  assert_int_equal(ret, 0);

  assert_int_equal(map_exists(m, key, strlen(key)), 1);

  ret = map_del(m, key, strlen(key));
  assert_int_equal(ret, 0);

  assert_int_equal(map_exists(m, key, strlen(key)), 0);
  size_t data_size = 12345;
  void *data = map_get(m, key, strlen(key), &data_size);
  assert_null(data);
  assert_int_equal(data_size, 0);

  map_destroy(m);
}

static void test_map_nonexistent(void **state)
{
  UNUSED(state);

  map_t *m = map_new(10);
  assert_non_null(m);

  const char *key = "nonexistent";
  size_t data_size = 99;
  void *data = map_get(m, key, strlen(key), &data_size);
  assert_null(data);
  free(data);
  assert_int_equal(data_size, 0);
  assert_int_equal(map_exists(m, key, strlen(key)), 0);

  int ret = map_del(m, key, strlen(key));
  assert_int_equal(ret, -1);

  map_destroy(m);
}

static void test_map_overflow(void **state)
{
  UNUSED(state);

  map_t *m = map_new(2);
  assert_non_null(m);

  const char *key1 = "a";
  const char *value1 = "data_a";
  const char *key2 = "b";
  const char *value2 = "data_b";
  const char *key3 = "c";
  const char *value3 = "data_c";

  int ret = map_set(m, key1, strlen(key1), value1, strlen(value1) + 1);
  assert_int_equal(ret, 0);
  ret = map_set(m, key2, strlen(key2), value2, strlen(value2) + 1);
  assert_int_equal(ret, 0);

  ret = map_set(m, key3, strlen(key3), value3, strlen(value3) + 1);
  assert_int_equal(ret, -1);

  map_destroy(m);
}

static void test_map_collision_resolution(void **state)
{
  UNUSED(state);

  map_t *m = map_new(3);
  assert_non_null(m);

  const char *key1 = "key1";
  const char *val1 = "value1";
  const char *key2 = "key2";
  const char *val2 = "value2";
  const char *key3 = "key3";
  const char *val3 = "value3";

  int ret = map_set(m, key1, strlen(key1), val1, strlen(val1) + 1);
  assert_int_equal(ret, 0);
  ret = map_set(m, key2, strlen(key2), val2, strlen(val2) + 1);
  assert_int_equal(ret, 0);
  ret = map_set(m, key3, strlen(key3), val3, strlen(val3) + 1);
  if (ret == -1)
  {
    fail_msg("Collision resolution failure: map_set returned -1 for key3");
  }

  assert_int_equal(map_exists(m, key1, strlen(key1)), 1);
  assert_int_equal(map_exists(m, key2, strlen(key2)), 1);
  assert_int_equal(map_exists(m, key3, strlen(key3)), 1);

  size_t out_size;
  char *ret_val1 = (char *)map_get(m, key1, strlen(key1), &out_size);
  assert_non_null(ret_val1);
  assert_string_equal(ret_val1, val1);
  free(ret_val1);

  char *ret_val2 = (char *)map_get(m, key2, strlen(key2), &out_size);
  assert_non_null(ret_val2);
  assert_string_equal(ret_val2, val2);
  free(ret_val2);

  char *ret_val3 = (char *)map_get(m, key3, strlen(key3), &out_size);
  assert_non_null(ret_val3);
  assert_string_equal(ret_val3, val3);
  free(ret_val3);

  map_destroy(m);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_map_create_destroy),
    cmocka_unit_test(test_map_set_get_exists),
    cmocka_unit_test(test_map_update),
    cmocka_unit_test(test_map_delete),
    cmocka_unit_test(test_map_nonexistent),
    cmocka_unit_test(test_map_overflow),
    cmocka_unit_test(test_map_collision_resolution),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
