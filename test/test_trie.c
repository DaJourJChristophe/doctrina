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
#include "trie.h"

static void test_trie_insert_search(void **state)
{
  UNUSED(state);

  trie_t *my_trie = NULL;
  my_trie = trie_new();
  assert_non_null(my_trie);

  trie_insert(my_trie, (const uint8_t *)"hello");
  trie_insert(my_trie, (const uint8_t *)"world");
  trie_insert(my_trie, (const uint8_t *)"hell");
  trie_insert(my_trie, (const uint8_t *)"!");

  assert_true(trie_search(my_trie, (const uint8_t *)"hello"));
  assert_true(trie_search(my_trie, (const uint8_t *)"world"));
  assert_true(trie_search(my_trie, (const uint8_t *)"hell"));
  assert_true(trie_search(my_trie, (const uint8_t *)"!"));

  assert_false(trie_search(my_trie, (const uint8_t *)"he"));
  assert_false(trie_search(my_trie, (const uint8_t *)"worl"));
  assert_false(trie_search(my_trie, (const uint8_t *)"world!"));

  trie_destroy(my_trie);
}

static void test_empty_string(void **state)
{
  UNUSED(state);

  trie_t *my_trie = NULL;
  my_trie = trie_new();
  assert_non_null(my_trie);

  assert_false(trie_search(my_trie, (const uint8_t *)""));

  trie_insert(my_trie, (const uint8_t *)"");

  assert_true(trie_search(my_trie, (const uint8_t *)""));

  trie_destroy(my_trie);
}

static void test_common_prefix(void **state)
{
  UNUSED(state);

  trie_t *my_trie = NULL;
  my_trie = trie_new();
  assert_non_null(my_trie);

  trie_insert(my_trie, (const uint8_t *)"test");
  trie_insert(my_trie, (const uint8_t *)"testing");

  assert_true(trie_search(my_trie, (const uint8_t *)"test"));
  assert_true(trie_search(my_trie, (const uint8_t *)"testing"));

  assert_false(trie_search(my_trie, (const uint8_t *)"tes"));

  trie_destroy(my_trie);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_trie_insert_search),
    cmocka_unit_test(test_empty_string),
    cmocka_unit_test(test_common_prefix),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
