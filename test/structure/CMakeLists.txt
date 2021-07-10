enable_testing()

set(SRC
  ${CMAKE_CURRENT_LIST_DIR}/structure.cc
  ${PROJECT_SOURCE_DIR}/src/eccentricity.cc
  )

add_executable(structure
  ${SRC}
  )

target_include_directories(structure PUBLIC
  "${PROJECT_SOURCE_DIR}/src/include"
  )

target_link_libraries(structure PUBLIC gtest gtest_main)

add_test(
  NAME structure
  COMMAND structure
  )

target_link_libraries(structure PUBLIC igraph)
