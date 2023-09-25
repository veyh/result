# https://github.com/google/sanitizers/wiki/MemorySanitizer

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  message(FATAL_ERROR "Memory Sanitizer not supported in MSVC")

# might not work on gcc either?

else()
  add_compile_options(
    -fsanitize=memory
    -fPIE
    -pie
    -fno-omit-frame-pointer
  )

  link_libraries(
    -fsanitize=memory
    -fPIE
    -pie
    -fno-omit-frame-pointer
  )
endif()
