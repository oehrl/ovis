file(GLOB OVIS_GRAPHICS_HEADER_FILES "*.hpp")
file(GLOB OVIS_GRAPHICS_SOURCE_FILES "*.cpp")

add_library(
  OvisGraphics
  
  blend_state.hpp
  cubemap.hpp cubemap.cpp
  depth_buffer_state.hpp
  gl.hpp
  graphics_buffer.hpp graphics_buffer.cpp
  graphics_context.hpp graphics_context.cpp
  graphics_resource.hpp graphics_resource.cpp
  index_buffer.hpp index_buffer.cpp
  render_target_configuration.hpp render_target_configuration.cpp
  render_target_texture2d.hpp render_target_texture2d.cpp
  render_target.hpp
  shader_program.hpp shader_program.cpp
  static_mesh.hpp
  stencil_state.hpp
  texture.hpp texture.cpp
  texture2d.hpp texture2d.cpp
  uniform_buffer.hpp uniform_buffer.cpp
  vertex_buffer.hpp vertex_buffer.cpp
  vertex_input.hpp vertex_input.cpp
)

if (APPLE)
  target_compile_definitions(
    OvisGraphics
    PUBLIC
      -DGL_SILENCE_DEPRECATION
  )
endif ()

find_package(OpenGL REQUIRED)
target_include_directories(
  OvisGraphics
  PUBLIC
    ${OVIS_ROOT}/extern/glm/include
)
target_link_libraries(
  OvisGraphics
  PUBLIC
    OpenGL::GL
    rapidjson
    OvisCore
)

if (WIN32)
  find_package(GLEW REQUIRED)
  target_include_directories(
    OvisGraphics
    PUBLIC
      ${GLEW_INCLUDE_DIRS}
  )
  target_link_libraries(
    OvisGraphics
    PUBLIC
      ${GLEW_LIBRARIES}
  )
endif ()
