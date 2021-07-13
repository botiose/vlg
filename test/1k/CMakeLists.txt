enable_testing()

set(SRC
  ${CMAKE_CURRENT_LIST_DIR}/1k.cc
  ${PROJECT_SOURCE_DIR}/src/eccentricity.cc
  ${PROJECT_SOURCE_DIR}/src/prune.cc
  )

add_executable(1k
  ${SRC}
  )

target_include_directories(1k PUBLIC
  "${PROJECT_SOURCE_DIR}/src/include"
  )

target_link_libraries(1k PUBLIC gtest gtest_main)

add_test(
  NAME 1k
  COMMAND 1k
  )

target_link_libraries(1k PUBLIC igraph)
