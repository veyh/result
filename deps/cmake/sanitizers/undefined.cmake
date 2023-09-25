# https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
#
# For stack traces with the errors, run the binary with environment variable
#
#   UBSAN_OPTIONS=print_stacktrace=1
#
# You can test that the sanitizer works with eg.
#
#   uint8_t a[4] = { 1, 2, 3, 4 };
#   uint16_t b = *(uint16_t *) &a[1];
#   printf("%u\n", b);
#
# That should say
#
#   runtime error: load of misaligned address 0x7ffe601a9421 for type
#   'uint16_t', which requires 2 byte alignment

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  message(FATAL_ERROR "Undefined Behavior Sanitizer not supported in MSVC")

else()
  add_compile_options(
    -fsanitize=undefined
    -fno-sanitize-recover=undefined
    -fno-omit-frame-pointer
  )

  link_libraries(
    -fsanitize=undefined
    -fno-sanitize-recover=undefined
    -fno-omit-frame-pointer
  )
endif()
