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
    ${CMAKE_SOURCE_DIR}/extern/glm/include
)
target_link_libraries(
  OvisMath
  PUBLIC
    OvisCore 
)
