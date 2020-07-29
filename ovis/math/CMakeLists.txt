add_library(
  OvisMath

  heightmap.hpp
  marching_cubes.hpp marching_cubes.cpp
  plane.hpp
  rect.hpp
  transform.hpp transform.cpp
  triangle.hpp
)
target_include_directories(
  OvisMath
  PUBLIC
    ${OVIS_ROOT}/extern/glm/include
)
message(STATUS ${OVIS_ROOT})
target_link_libraries(
  OvisMath
  PUBLIC
    OvisCore 
)
