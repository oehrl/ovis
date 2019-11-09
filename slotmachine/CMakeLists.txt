file(GLOB SLOT_MACHINE_HEADER_FILES "*.hpp")
file(GLOB SLOT_MACHINE_SOURCE_FILES "*.cpp")

if (OVIS_EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_executable(SlotMachine ${SLOT_MACHINE_HEADER_FILES}
                           ${SLOT_MACHINE_SOURCE_FILES})

add_custom_command(
  TARGET SlotMachine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/resources/* ${CMAKE_CURRENT_BINARY_DIR}/resources
)

target_link_libraries(SlotMachine OvisEngine)
if (OVIS_EMSCRIPTEN)
  add_compile_options(SlotMachine "--preload-file resources")
endif (OVIS_EMSCRIPTEN)