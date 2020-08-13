add_library(
  OvisEngine
  
  engine.hpp engine.cpp
  window.hpp window.cpp
  camera.hpp camera.cpp
  scene_component.hpp
  scene_controller.hpp scene_controller.cpp
  scene_object.hpp scene_object.cpp
  scene.hpp scene.cpp
  module.hpp module.cpp
  render_pipeline.hpp render_pipeline.cpp
  render_pass.hpp render_pass.cpp
)
target_link_libraries(
  OvisEngine
  PUBLIC
    OvisCore
    OvisMath
    OvisGraphics
)
