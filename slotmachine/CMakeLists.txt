file(GLOB SLOT_MACHINE_HEADER_FILES "*.hpp")
file(GLOB SLOT_MACHINE_SOURCE_FILES "*.cpp")

if (OVIS_EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_executable(
  SlotMachine
  ${SLOT_MACHINE_HEADER_FILES}
  ${SLOT_MACHINE_SOURCE_FILES}
)

# add_custom_command(
#   TARGET SlotMachine
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/resources/* ${CMAKE_CURRENT_BINARY_DIR}/resources
# )

# function(add_texture2d FILE_PATH)
#   get_filename_component(FILE_DIRECTORY ${FILE_PATH} DIRECTORY)
#   get_filename_component(FILE_NAME ${FILE_PATH} NAME_WE)

#   SET(INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/${FILE_PATH})
#   SET(OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/resources/${FILE_DIRECTORY}/${FILE_NAME}.texture2d)

#   add_custom_command(
#     OUTPUT ${OUTPUT_FILE} ${OUTPUT_FILE}.json
#     COMMAND OvisTextureConverter 2d ${INPUT_FILE} ${OUTPUT_FILE}
#     DEPENDS ${INPUT_FILE}
#   )
#   string(
#     UUID TARGET_NAME
#     NAMESPACE "73203eb4-80d3-4957-a110-8aae92c7e615"
#     NAME ${FILE_PATH}
#     TYPE SHA1
#   )
#   add_custom_target(SlotMachineResources-${TARGET_NAME} ALL DEPENDS ${OUTPUT_FILE} ${OUTPUT_FILE}.json)
# endfunction()

# function(add_shader SHADER_BASE_PATH)
#   set(VERTEX_SHADER_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/${SHADER_BASE_PATH}.vert)
#   set(VERTEX_SHADER_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources/${SHADER_BASE_PATH}.vert)

#   set(FRAGMENT_SHADER_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/${SHADER_BASE_PATH}.frag)
#   set(FRAGMENT_SHADER_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources/${SHADER_BASE_PATH}.frag)

#   set(PARAMETER_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/resources/${SHADER_BASE_PATH}.shader.json)

#   add_custom_command(
#     OUTPUT ${VERTEX_SHADER_OUTPUT}
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VERTEX_SHADER_INPUT} ${VERTEX_SHADER_OUTPUT}
#     DEPENDS ${VERTEX_SHADER_INPUT}
#   )

#   add_custom_command(
#     OUTPUT ${FRAGMENT_SHADER_OUTPUT}
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FRAGMENT_SHADER_INPUT} ${FRAGMENT_SHADER_OUTPUT}
#     DEPENDS ${FRAGMENT_SHADER_INPUT}
#   )

  
#   add_custom_command(
#     OUTPUT ${PARAMETER_OUTPUT_FILE}
#     COMMAND ${CMAKE_COMMAND} -E echo "{\"vertex_shader_source\":\"sprite.vert\",\"fragment_shader_source\": \"sprite.frag\"}" > ${PARAMETER_OUTPUT_FILE}
#   )
# endfunction()

# add_texture2d(icons/beer.png)
# add_texture2d(icons/push_up.png)
# add_texture2d(icons/squat.png)
# add_texture2d(icons/shot.png)
# add_shader(sprite)

add_custom_target(
  SlotMachineResources
  OvisResourceCompiler
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources  
)

target_link_libraries(
  SlotMachine
  PUBLIC
    OvisEngine
)
add_dependencies(SlotMachine SlotMachineResources)

if (OVIS_EMSCRIPTEN)
  add_compile_options(SlotMachine "--preload-file resources")
endif (OVIS_EMSCRIPTEN)