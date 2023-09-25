if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(
    /fsanitize=address
  )

  link_libraries(
    /fsanitize=address
  )

else()
  add_compile_options(
    -fsanitize=address,undefined
    -fno-sanitize-recover=undefined
    -fno-omit-frame-pointer
  )

  link_libraries(
    -fsanitize=address,undefined
    -fno-sanitize-recover=undefined
    -fno-omit-frame-pointer
  )
endif()
