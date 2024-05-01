add_executable(sitef-basic-tests_tests test.cpp)
target_link_libraries(sitef-basic-tests_tests sitef-basic-tests_lib GTest::GTest GTest::Main)
gtest_discover_tests(sitef-basic-tests_tests)
