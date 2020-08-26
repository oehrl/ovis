add_library(
  OvisEngine
  
  engine.hpp engine.cpp
  window.hpp window.cpp
  viewport.hpp viewport.cpp
  camera.hpp camera.cpp
  scene_object_component.hpp scene_object_component.cpp
  scene_controller.hpp scene_controller.cpp
  scene_object.hpp scene_object.cpp
  scene.hpp scene.cpp
  module.hpp module.cpp
  render_pipeline.hpp render_pipeline.cpp
  render_pass.hpp render_pass.cpp

  base/base_module.hpp base/base_module.cpp
  base/imgui_render_pass.hpp base/imgui_render_pass.cpp
  base/imgui_scene_controller.hpp base/imgui_scene_controller.cpp
  base/transform2d_component.hpp base/transform2d_component.cpp
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
if (OVIS_EMSCRIPTEN)
  target_link_options(
  OvisEngine
  PUBLIC
    --preload-file ${CMAKE_BINARY_DIR}/resources@/resources
  )
endif ()