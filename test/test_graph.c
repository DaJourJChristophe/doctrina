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

#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "deque.h"
#include "graph.h"
#include "map.h"
#include "set.h"
#include "stack.h"

/*
* test_graph_create_destroy:
*   Verify that a graph can be created and destroyed without error.
*/
static void test_graph_create_destroy(void **state) {
    UNUSED(state);
    graph_t *g = graph_create(16);
    assert_non_null(g);
    // Simply destroy the graph; if any internal memory management errors occur,
    // AddressSanitizer or valgrind will report them.
    graph_destroy(g);
}

/*
* test_graph_add_edge:
*   Construct a graph by adding two edges from the same source node.
*   Then, by accessing the internal map and set, verify that:
*   - A node corresponding to the source data exists.
*   - Its edge set contains exactly 2 edges.
*   - The destination nodes have the expected data.
*/
static void test_graph_add_edge(void **state) {
    UNUSED(state);
    graph_t *g = graph_create(16);
    assert_non_null(g);

    uint32_t a = 5, b = 6, c = 7;
    // Add two edges from node 'a' to nodes 'b' and 'c'
    graph_add_edge(g, &a, sizeof(a), &b, sizeof(b));
    graph_add_edge(g, &a, sizeof(a), &c, sizeof(c));

    /* Retrieve the graph node for data 'a' from the internal map.
    * Note: Our graph_add_node implementation uses the data as key.
    */
    size_t dummy_size = 0;
    // map_get returns a dynamically allocated buffer containing the stored node pointer.
    void *ret = map_get(g->nodes, &a, sizeof(a), &dummy_size);
    assert_non_null(ret);
    // Extract the pointer to the graph_node from the returned buffer.
    graph_node_t *node = *(graph_node_t **)ret;
    free(ret);
    assert_non_null(node);

    /* Obtain the list of edge pointers stored in the node's edge set.
    * set_getall returns a contiguous block of memory containing the stored keys.
    * In our usage, each key is a pointer to a graph_edge_t, so the total number
    * of edges is overall_size / sizeof(graph_edge_t *).
    */
    size_t overall_size = 0;
    void *edge_buf = set_getall(node->edges, &overall_size);
    assert_non_null(edge_buf);
    size_t num_edges = overall_size / sizeof(graph_edge_t *);
    // Expect 2 edges.
    assert_int_equal(num_edges, 2);

    graph_edge_t **edges = (graph_edge_t **)edge_buf;
    bool found_b = false, found_c = false;
    for (size_t i = 0; i < num_edges; i++) {
        assert_non_null(edges[i]);
        graph_node_t *dest_node = edges[i]->dest;
        assert_non_null(dest_node);
        uint32_t dest_val = *(uint32_t *)dest_node->data;
        if (dest_val == b) {
            found_b = true;
        } else if (dest_val == c) {
            found_c = true;
        }
    }
    free(edge_buf);
    assert_true(found_b && found_c);

    graph_destroy(g);
}

/*
* test_graph_bfs:
*   Create a small graph and perform breadth-first search.
*   This test verifies that the BFS traversal completes without error.
*/
static void test_graph_bfs(void **state) {
    UNUSED(state);
    graph_t *g = graph_create(16);
    assert_non_null(g);

    /* Construct a small graph:
    *       1
    *      / \
    *     2   3
    *      \ /
    *       4
    */
    uint32_t one = 1, two = 2, three = 3, four = 4;
    graph_add_edge(g, &one, sizeof(one), &two, sizeof(two));
    graph_add_edge(g, &one, sizeof(one), &three, sizeof(three));
    graph_add_edge(g, &two, sizeof(two), &four, sizeof(four));
    graph_add_edge(g, &three, sizeof(three), &four, sizeof(four));

    /* Run BFS starting from node '1'. Since our BFS function does not
    * produce an output (it only uses internal queues to traverse the graph),
    * we only verify that it completes without an error (or abnormal exit).
    */
    graph_bfs(g, &one, sizeof(one));
    graph_destroy(g);
}

/*
* test_graph_dfs:
*   Create a small graph and perform depth-first search.
*   Similar to BFS, this test verifies that DFS completes without error.
*/
static void test_graph_dfs(void **state) {
    UNUSED(state);
    graph_t *g = graph_create(16);
    assert_non_null(g);

    /* Construct the same small graph as in BFS test. */
    uint32_t one = 1, two = 2, three = 3, four = 4;
    graph_add_edge(g, &one, sizeof(one), &two, sizeof(two));
    graph_add_edge(g, &one, sizeof(one), &three, sizeof(three));
    graph_add_edge(g, &two, sizeof(two), &four, sizeof(four));
    graph_add_edge(g, &three, sizeof(three), &four, sizeof(four));

    graph_dfs(g, &one, sizeof(one));
    graph_destroy(g);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_graph_create_destroy),
    cmocka_unit_test(test_graph_add_edge),
    cmocka_unit_test(test_graph_bfs),
    cmocka_unit_test(test_graph_dfs),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
