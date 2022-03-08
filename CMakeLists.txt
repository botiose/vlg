cmake_minimum_required(VERSION 3.10)
project(be)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(SRC
  src/main.cc
  src/eccentricity.cc
  src/prune.cc
  )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_source_files_properties(${SRC}
    PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -Werror -g"
    )
endif ()

add_executable(be
  ${SRC}
  )

target_include_directories(be PUBLIC
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

add_custom_target(etc-link
  [ ! -d etc ] && ln -s ${PROJECT_SOURCE_DIR}/etc ${PROJECT_BINARY_DIR}/etc ||
  exit 0
  )

# RUN TARGET
add_custom_target(run-debug
  DEPENDS be
  DEPENDS etc-link
  COMMAND printf %37sOUTPUT%37s\\\\n | tr " " "="
  COMMAND valgrind --leak-check=full --show-leak-kinds=all
  --suppressions=etc/valgrind.supp --log-file=cur-leaks.log ./bin/be
  COMMAND printf %38sLEAK%38s\\\\n | tr " " "="
  COMMAND ./etc/script/stat-leaks.sh
  COMMAND printf %80s\\\\n | tr " " "="
  )

add_custom_target(run
  DEPENDS etc-link
  COMMAND printf %37sOUTPUT%37s\\\\n | tr " " "="
  COMMAND ./bin/be etc/data/ca-HepTh.txt
  COMMAND printf %80s\\\\n | tr " " "="
  DEPENDS be
  )

# RUN PROFILE TARGET
add_custom_target(run-profile
  DEPENDS be
  COMMAND printf %37sOUTPUT%37s\\\\n | tr " " "="
  COMMAND
  valgrind --tool=callgrind --collect-systime=yes --separate-threads=yes
  --instr-atstart=no ./bin/be
  COMMAND printf %38sLEAK%38s\\\\n | tr " " "="
  COMMAND printf %80s\\\\n | tr " " "="
  )

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_subdirectory(ext/igraph-0.9.4 EXCLUDE_FROM_ALL)

target_link_libraries(be PUBLIC igraph)
