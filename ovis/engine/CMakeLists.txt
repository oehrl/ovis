add_library(
  OvisEngine
  
  engine.hpp engine.cpp
  window.hpp window.cpp
)
target_link_libraries(
  OvisEngine
  PUBLIC
    OvisCore
    OvisGraphics
    OvisScene
    OvisRendering
)
