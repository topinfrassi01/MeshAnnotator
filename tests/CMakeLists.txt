find_package(Catch2 CONFIG REQUIRED)

add_executable(unit_tests
  test_main.cpp
)

target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME unit_tests COMMAND unit_tests)
