cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(result VERSION 1.0.0 LANGUAGES C)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(deps/cmake/sanitizers/all.cmake)
endif()

include(deps/cmake/c99-gnu.cmake)
include_directories(deps)
include_directories("${PROJECT_BINARY_DIR}")

if (PROJECT_IS_TOP_LEVEL AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_testing()
  add_compile_options(-D UNIT_TESTING)

  include(deps/cmake/cmocka-git.cmake)

  add_cmocka_test(result_test
    SOURCES result_test.c
    LINK_LIBRARIES cmocka-static
  )
endif()
