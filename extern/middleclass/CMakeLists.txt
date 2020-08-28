file(READ middleclass.lua MIDDLECLASS_SOURCE)

configure_file(
    middleclass.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/middleclass.hpp
)

add_library(
  middleclass
  INTERFACE
)

target_include_directories(
  middleclass
  INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}
)
