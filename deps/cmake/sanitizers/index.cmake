if (CMAKE_CROSSCOMPILING OR NOT BUILD_TESTING)
  set(sanitizer_type_default "none")
else()
  set(sanitizer_type_default "all")
endif()

set(
  SANITIZER_TYPE
  "${sanitizer_type_default}" CACHE STRING
  "Choose a sanitizer to use"
)

set_property(
  CACHE SANITIZER_TYPE
  PROPERTY STRINGS
    none
    all
    address
    memory
    undefined
)

if (SANITIZER_TYPE AND NOT SANITIZER_TYPE STREQUAL "none")
  include("${CMAKE_CURRENT_LIST_DIR}/${SANITIZER_TYPE}.cmake")
endif()
