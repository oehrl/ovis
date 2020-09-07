add_library(
  OvisRendering2D
  
  rendering2d_module.hpp rendering2d_module.cpp
  sprite_renderer.hpp sprite_renderer.cpp
  sprite_component.hpp sprite_component.cpp
)
target_link_libraries(
  OvisRendering2D
  PUBLIC
    OvisEngine
)
add_custom_command(
  TARGET OvisRendering2D
  PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
)
target_link_options(
 OvisRendering2D
 PUBLIC
  --preload-file ${CMAKE_BINARY_DIR}/resources@/resources
)
