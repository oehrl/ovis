file(GLOB WATER_HEADER_FILES "*.hpp")
file(GLOB WATER_SOURCE_FILES "*.cpp")

if (OVIS_EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_executable(Water ${WATER_HEADER_FILES}
                     ${WATER_SOURCE_FILES})

add_custom_command(
  TARGET Water
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/resources/* ${CMAKE_CURRENT_BINARY_DIR}/resources
)

target_link_libraries(Water OvisEngine)