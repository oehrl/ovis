add_library(
  OvisScripting
  
  scripting_module.hpp scripting_module.cpp
  script_scene_controller.hpp script_scene_controller.cpp
)
target_link_libraries(
  OvisScripting
  PUBLIC
    OvisEngine
    Lua
    sol2_single
    middleclass
)
