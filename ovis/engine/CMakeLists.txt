add_library(
  OvisEngine
  
  engine.hpp engine.cpp
  window.hpp window.cpp
  viewport.hpp viewport.cpp
  camera.hpp camera.cpp
  scene_component.hpp
  scene_controller.hpp scene_controller.cpp
  scene_object.hpp scene_object.cpp
  scene.hpp scene.cpp
  module.hpp module.cpp
  render_pipeline.hpp render_pipeline.cpp
  render_pass.hpp render_pass.cpp

  imgui/imgui_module.hpp imgui/imgui_module.cpp
  imgui/imgui_render_pass.hpp imgui/imgui_render_pass.cpp
  imgui/imgui_scene_controller.hpp imgui/imgui_scene_controller.cpp
)
target_link_libraries(
  OvisEngine
  PUBLIC
    OvisCore
    OvisMath
    OvisGraphics
    DearImGui
)
add_custom_command(
  TARGET OvisEngine
  PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
)
target_link_options(
 OvisEngine
 PUBLIC
  --preload-file ${CMAKE_BINARY_DIR}/resources
)
