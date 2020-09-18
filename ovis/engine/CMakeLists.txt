add_library(
  OvisEngine
  
  asset_library.hpp asset_library.cpp
  engine.hpp engine.cpp
  viewport.hpp viewport.cpp
  window.hpp window.cpp
  render_target_viewport.hpp render_target_viewport.cpp
  camera.hpp camera.cpp
  scene_object_component.hpp scene_object_component.cpp
  scene_controller.hpp scene_controller.cpp
  script_scene_controller.hpp script_scene_controller.cpp
  scene_object.hpp scene_object.cpp
  scene.hpp scene.cpp
  module.hpp module.cpp
  render_pass.hpp render_pass.cpp
  lua.hpp lua.cpp

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
    DearImGui    Lua
    sol2_single
    middleclass
)
add_custom_command(
  TARGET OvisEngine
  PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
target_link_options(
 OvisEngine
 PUBLIC
  --preload-file ${CMAKE_BINARY_DIR}/assets@/ovis_assets
)
