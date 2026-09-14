add_executable(chat_tests stub_test.cpp)
target_link_libraries(chat_tests PRIVATE chat_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(chat_tests)