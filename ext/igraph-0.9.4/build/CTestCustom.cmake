# Ask CTest to run the build_tests target before running the tests
set(CTEST_CUSTOM_PRE_TEST "/usr/bin/cmake --build /home/otiose/repos/epita/vlg/ext/igraph-0.9.4/build --target build_tests")
