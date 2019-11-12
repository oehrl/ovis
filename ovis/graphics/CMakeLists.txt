file(GLOB OVIS_GRAPHICS_HEADER_FILES "*.hpp")
file(GLOB OVIS_GRAPHICS_SOURCE_FILES "*.cpp")

add_library(
  OvisGraphics
  ${OVIS_GRAPHICS_HEADER_FILES}
  ${OVIS_GRAPHICS_SOURCE_FILES}
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
    "${CMAKE_SOURCE_DIR}/extern/SDL/include"
    ${OPENGL_INCLUDE_DIRS}
)
target_link_libraries(
  OvisGraphics
  PUBLIC
    SDL2
    ${OPENGL_LIBRARIES}
    rapidjson
)
set_target_properties(
  OvisGraphics
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
