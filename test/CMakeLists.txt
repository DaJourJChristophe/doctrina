add_executable(test_deque
  "${CMAKE_CURRENT_SOURCE_DIR}/test_deque.c"
)

target_link_libraries(test_deque PRIVATE asan)
target_link_libraries(test_deque PRIVATE cmocka)
target_link_libraries(test_deque PRIVATE doctrina)

add_executable(test_heap
  "${CMAKE_CURRENT_SOURCE_DIR}/test_heap.c"
)

target_link_libraries(test_heap PRIVATE asan)
target_link_libraries(test_heap PRIVATE cmocka)
target_link_libraries(test_heap PRIVATE doctrina)

add_executable(test_map
  "${CMAKE_CURRENT_SOURCE_DIR}/test_map.c"
)

target_link_libraries(test_map PRIVATE asan)
target_link_libraries(test_map PRIVATE cmocka)
target_link_libraries(test_map PRIVATE doctrina)

add_executable(test_pq
  "${CMAKE_CURRENT_SOURCE_DIR}/test_pq.c"
)

target_link_libraries(test_pq PRIVATE asan)
target_link_libraries(test_pq PRIVATE cmocka)
target_link_libraries(test_pq PRIVATE doctrina)

add_executable(test_set
  "${CMAKE_CURRENT_SOURCE_DIR}/test_set.c"
)

target_link_libraries(test_set PRIVATE asan)
target_link_libraries(test_set PRIVATE cmocka)
target_link_libraries(test_set PRIVATE doctrina)

add_executable(test_stack
  "${CMAKE_CURRENT_SOURCE_DIR}/test_stack.c"
)

target_link_libraries(test_stack PRIVATE asan)
target_link_libraries(test_stack PRIVATE cmocka)
target_link_libraries(test_stack PRIVATE doctrina)

add_executable(test_trie
  "${CMAKE_CURRENT_SOURCE_DIR}/test_trie.c"
)

target_link_libraries(test_trie PRIVATE asan)
target_link_libraries(test_trie PRIVATE cmocka)
target_link_libraries(test_trie PRIVATE doctrina)
