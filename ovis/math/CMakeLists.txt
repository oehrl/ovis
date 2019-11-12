file(GLOB OVIS_MATH_HEADER_FILES "*.hpp")
file(GLOB OVIS_MATH_SOURCE_FILES "*.cpp")

add_library(
  OvisMath
  ${OVIS_MATH_HEADER_FILES}
  ${OVIS_MATH_SOURCE_FILES}
)
set_target_properties(
  OvisMath
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
