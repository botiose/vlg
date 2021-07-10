enable_testing()

set(SRC
  ${CMAKE_CURRENT_LIST_DIR}/1m.cc
  ${PROJECT_SOURCE_DIR}/src/eccentricity.cc
  )

add_executable(1m
  ${SRC}
  )

target_include_directories(1m PUBLIC
  "${PROJECT_SOURCE_DIR}/src/include"
  )

target_link_libraries(1m PUBLIC gtest gtest_main)

add_test(
  NAME 1m
  COMMAND 1m
  )

target_link_libraries(1m PUBLIC igraph)
