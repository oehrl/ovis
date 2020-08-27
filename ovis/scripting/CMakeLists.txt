add_library(
  OvisScripting
  
  scripting_module.hpp scripting_module.cpp
)
target_link_libraries(
  OvisScripting
  PUBLIC
    OvisEngine
    Lua
    sol2_single
)
