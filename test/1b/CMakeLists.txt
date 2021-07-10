enable_testing()

set(SRC
  ${CMAKE_CURRENT_LIST_DIR}/1b.cc
  ${PROJECT_SOURCE_DIR}/src/eccentricity.cc
  )

add_executable(1b
  ${SRC}
  )

target_include_directories(1b PUBLIC
  "${PROJECT_SOURCE_DIR}/src/include"
  )

target_link_libraries(1b PUBLIC gtest gtest_main)

add_test(
  NAME 1b
  COMMAND 1b
  )

target_link_libraries(1b PUBLIC igraph)
