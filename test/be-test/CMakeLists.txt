enable_testing()

set(SRC
  ${CMAKE_CURRENT_LIST_DIR}/be.cc
  # ${PROJECT_SOURCE_DIR}/src/be.cc
  )

add_executable(be-test
  ${SRC}
  )

target_include_directories(be-test PUBLIC
  "${PROJECT_SOURCE_DIR}/src/include"
  )

target_link_libraries(be-test PUBLIC gtest gtest_main)

add_test(
  NAME be-test
  COMMAND be-test
  )

