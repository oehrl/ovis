file(GLOB OVIS_GRAPHICS_HEADER_FILES "*.hpp")
file(GLOB OVIS_GRAPHICS_SOURCE_FILES "*.cpp")

add_library(OvisGraphics
  ${OVIS_GRAPHICS_HEADER_FILES}
  ${OVIS_GRAPHICS_SOURCE_FILES})

find_package(OpenGL REQUIRED)

if (APPLE)
  target_compile_definitions(OvisGraphics
    PUBLIC -DGL_SILENCE_DEPRECATION)
endif ()

target_include_directories(OvisGraphics PUBLIC "${CMAKE_SOURCE_DIR}/extern/SDL/include" ${OPENGL_INCLUDE_DIRS})
target_link_libraries(OvisGraphics PUBLIC SDL2 ${OPENGL_LIBRARIES} rapidjson)
