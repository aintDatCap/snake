add_executable(TestQueue test_queue.cpp)
target_link_libraries(TestQueue Check::check)
target_include_directories(TestQueue PUBLIC ${SNAKE_SOURCE_DIR})