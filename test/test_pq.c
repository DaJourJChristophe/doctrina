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
#include "pq.h"

/* A simple integer comparison function for testing.
 * Returns a negative value if *a < *b,
 * zero if *a == *b and a positive value if *a > *b.
 */
static int int_compare(const void *a, const void *b)
{
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    if (int_a < int_b)
        return -1;
    else if (int_a > int_b)
        return 1;
    else
        return 0;
}

/* Test that a newly created priority queue is empty. */
static void test_pq_empty(void **state)
{
    (void) state;
    priority_queue_t *pq = pq_new(int_compare);
    assert_non_null(pq);
    assert_null(pq_peek(pq));
    assert_null(pq_pop(pq));
    pq_destroy(pq);
}

/* Test that pushing a single element behaves as expected:
 * - peek returns the element without removal
 * - pop returns the element and leaves the queue empty.
 */
static void test_pq_single(void **state)
{
    (void) state;
    priority_queue_t *pq = pq_new(int_compare);
    assert_non_null(pq);

    int value = 42;
    int ret = pq_push(pq, &value);
    assert_int_equal(ret, 0);

    void *peek_value = pq_peek(pq);
    assert_non_null(peek_value);
    assert_int_equal(*(int *)peek_value, 42);

    void *pop_value = pq_pop(pq);
    assert_non_null(pop_value);
    assert_int_equal(*(int *)pop_value, 42);

    /* After removal, the queue is empty. */
    assert_null(pq_peek(pq));
    pq_destroy(pq);
}

/* Test that when multiple elements are pushed the priority queue returns
 * the minimum element on each pop.
 */
static void test_pq_order(void **state)
{
    (void) state;
    priority_queue_t *pq = pq_new(int_compare);
    assert_non_null(pq);

    /* Insert a series of integers (order is arbitrary) */
    int values[] = { 10, 5, 3, 7, 2, 8 };
    size_t count = sizeof(values) / sizeof(values[0]);
    for (size_t i = 0; i < count; i++) {
        int ret = pq_push(pq, &values[i]);
        assert_int_equal(ret, 0);
    }

    /* Expected sorted order for a min-queue */
    int expected[] = { 2, 3, 5, 7, 8, 10 };
    for (size_t i = 0; i < count; i++) {
        void *pop_value = pq_pop(pq);
        assert_non_null(pop_value);
        int actual = *(int *)pop_value;
        assert_int_equal(actual, expected[i]);
    }
    assert_null(pq_peek(pq));
    pq_destroy(pq);
}

/* Test merging of two priority queues.
 * Create two distinct queues, insert elements in each, merge them,
 * and verify the merged queue yields the expected sorted order.
 */
static void test_pq_merge(void **state)
{
    (void) state;
    /* Create first priority queue and add elements */
    priority_queue_t *pq1 = pq_new(int_compare);
    assert_non_null(pq1);
    int a = 10, b = 15, c = 5;
    assert_int_equal(pq_push(pq1, &a), 0);
    assert_int_equal(pq_push(pq1, &b), 0);
    assert_int_equal(pq_push(pq1, &c), 0);

    /* Create second priority queue and add elements */
    priority_queue_t *pq2 = pq_new(int_compare);
    assert_non_null(pq2);
    int d = 3, e = 8, f = 12;
    assert_int_equal(pq_push(pq2, &d), 0);
    assert_int_equal(pq_push(pq2, &e), 0);
    assert_int_equal(pq_push(pq2, &f), 0);

    /* Merge pq2 into pq1; pq_merge frees pq2 internally. */
    priority_queue_t *merged = pq_merge(pq1, pq2);
    assert_non_null(merged);

    int expected[] = { 3, 5, 8, 10, 12, 15 };
    size_t count = sizeof(expected) / sizeof(expected[0]);
    for (size_t i = 0; i < count; i++) {
        void *pop_value = pq_pop(merged);
        assert_non_null(pop_value);
        int actual = *(int *)pop_value;
        assert_int_equal(actual, expected[i]);
    }
    assert_null(pq_peek(merged));
    pq_destroy(merged);
}

/* Test that repeated calls to peek (poll) do not remove the element.
 * Only a pop will remove the top element.
 */
static void test_pq_peek_consistency(void **state)
{
    (void) state;
    priority_queue_t *pq = pq_new(int_compare);
    assert_non_null(pq);

    int value = 42;
    assert_int_equal(pq_push(pq, &value), 0);

    /* Multiple calls to peek should yield the same element */
    void *first_peek = pq_peek(pq);
    assert_non_null(first_peek);
    assert_int_equal(*(int *)first_peek, 42);

    void *second_peek = pq_peek(pq);
    assert_non_null(second_peek);
    assert_int_equal(*(int *)second_peek, 42);

    /* After a pop, the element is removed */
    void *pop_value = pq_pop(pq);
    assert_non_null(pop_value);
    assert_int_equal(*(int *)pop_value, 42);
    assert_null(pq_peek(pq));

    pq_destroy(pq);
}

/* Test merging behavior when one or both queues are NULL.
 * Merging a valid queue with NULL should return the valid queue.
 */
static void test_pq_merge_null(void **state)
{
    (void) state;
    /* Case 1: Merge pq1 and NULL */
    priority_queue_t *pq1 = pq_new(int_compare);
    assert_non_null(pq1);
    int value = 100;
    assert_int_equal(pq_push(pq1, &value), 0);
    priority_queue_t *merged1 = pq_merge(pq1, NULL);
    assert_non_null(merged1);
    void *peek_val = pq_peek(merged1);
    assert_non_null(peek_val);
    assert_int_equal(*(int *)peek_val, 100);
    pq_destroy(merged1);

    /* Case 2: Merge NULL and pq2 */
    priority_queue_t *pq2 = pq_new(int_compare);
    assert_non_null(pq2);
    int value2 = 200;
    assert_int_equal(pq_push(pq2, &value2), 0);
    priority_queue_t *merged2 = pq_merge(NULL, pq2);
    assert_non_null(merged2);
    peek_val = pq_peek(merged2);
    assert_non_null(peek_val);
    assert_int_equal(*(int *)peek_val, 200);
    pq_destroy(merged2);
}

/* Test that pushing into a NULL priority queue returns an error (-1). */
static void test_pq_push_null(void **state)
{
    (void) state;
    int value = 50;
    int ret = pq_push(NULL, &value);
    assert_int_equal(ret, -1);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_pq_empty),
    cmocka_unit_test(test_pq_single),
    cmocka_unit_test(test_pq_order),
    cmocka_unit_test(test_pq_merge),
    cmocka_unit_test(test_pq_peek_consistency),
    cmocka_unit_test(test_pq_merge_null),
    cmocka_unit_test(test_pq_push_null)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
