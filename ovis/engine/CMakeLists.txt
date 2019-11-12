file(GLOB OVIS_ENGINE_HEADER_FILES "*.hpp")
file(GLOB OVIS_ENGINE_SOURCE_FILES "*.cpp")

add_library(
  OvisEngine
  ${OVIS_ENGINE_HEADER_FILES}
  ${OVIS_ENGINE_SOURCE_FILES}
)
target_link_libraries(
  OvisEngine
  PUBLIC
    OvisCore
    OvisGraphics
    OvisScene
    OvisGui
)
set_target_properties(
  OvisEngine
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)