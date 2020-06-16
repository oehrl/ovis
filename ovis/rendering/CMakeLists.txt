add_library(
  OvisRendering
  render_pass.hpp
  render_pipeline.hpp
  render_pipeline.cpp
)

target_link_libraries(
  OvisRendering
  PUBLIC
    OvisCore
    OvisGraphics
    OvisScene
)
