add_library(
  OvisScene
  
  camera.hpp camera.cpp
  scene_component.hpp
  scene_controller.hpp scene_controller.cpp
  scene_object.hpp scene_object.cpp
  scene.hpp scene.cpp
  sub_system.hpp sub_system.cpp
)
target_link_libraries(
  OvisScene
  PUBLIC
    OvisCore
    OvisMath
    OvisGraphics
)
