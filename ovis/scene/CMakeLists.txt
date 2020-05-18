file(GLOB OVIS_SCENE_HEADER_FILES "*.hpp")
file(GLOB OVIS_SCENE_SOURCE_FILES "*.cpp")

add_library(
  OvisScene
  ${OVIS_SCENE_HEADER_FILES}
  ${OVIS_SCENE_SOURCE_FILES}
)
target_link_libraries(
  OvisScene
  PUBLIC
    OvisCore
    OvisMath
    OvisGraphics
)
