enable_testing()

add_subdirectory(structure EXCLUDE_FROM_ALL)
add_subdirectory(1k EXCLUDE_FROM_ALL)
add_subdirectory(1m EXCLUDE_FROM_ALL)
add_subdirectory(1b EXCLUDE_FROM_ALL)

add_custom_target(check
  COMMAND printf %80s\\\\n | tr " " "="
  COMMAND ${CMAKE_CTEST_COMMAND} || exit 0
  DEPENDS structure
  DEPENDS 1k
  DEPENDS 1m
  DEPENDS 1b
  COMMAND printf %80s\\\\n | tr " " "="
  )

add_custom_target(check-verbose
  COMMAND printf %80s\\\\n | tr " " "="
  COMMAND GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --output-on-failure |
  grep -v Running.* || exit 0 
  DEPENDS structure
  DEPENDS 1k
  DEPENDS 1m
  DEPENDS 1b
  COMMAND printf %80s\\\\n | tr " " "="
  )
