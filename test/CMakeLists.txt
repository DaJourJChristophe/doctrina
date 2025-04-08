add_executable(test_deque
  "${CMAKE_CURRENT_SOURCE_DIR}/test_deque.c"
)

target_link_libraries(test_deque PRIVATE asan)
target_link_libraries(test_deque PRIVATE cmocka)
target_link_libraries(test_deque PRIVATE doctrina)
