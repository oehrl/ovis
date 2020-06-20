add_library(
  OvisRendering
  render_pipeline.hpp
  render_pipeline.cpp
  render_pass.hpp
  render_pass.cpp
  geometry_pass.hpp
  geometry_pass.cpp
)

target_link_libraries(
  OvisRendering
  PUBLIC
    OvisCore
    OvisGraphics
    OvisScene
)
