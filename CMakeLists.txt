cmake_minimum_required(VERSION 3.10)
project(template)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(SRC
  src/main.cc
  src/template.cc
  )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_source_files_properties(${SRC}
    PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -Werror -g"
    )
endif ()

add_executable(template
  ${SRC}
  )

target_include_directories(template PUBLIC
  "${PROJECT_SOURCE_DIR}/src/include"
  )

# DOCUMENTATION SYMBOLIC LINK
add_custom_target(doc-link
  [ ! -d doc ] && ln -s ${PROJECT_SOURCE_DIR}/doc ${PROJECT_BINARY_DIR}/doc ||
  exit 0
  )

# DOCUMENTATION GENERATE
add_custom_target(doc
  COMMAND doxygen doc/Doxyfile && firefox --new-window html/index.html
  DEPENDS doc-link
  )

# RUN TARGET
add_custom_target(run
  COMMAND ./bin/template
  DEPENDS template
  )

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_subdirectory(test/extern/googletest EXCLUDE_FROM_ALL)
add_subdirectory(test EXCLUDE_FROM_ALL)



