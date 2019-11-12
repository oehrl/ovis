file(GLOB OVIS_CORE_HEADER_FILES "*.hpp")
file(GLOB OVIS_CORE_SOURCE_FILES "*.cpp")

find_package(SDL2 REQUIRED)

add_library(
  OvisCore
  ${OVIS_CORE_HEADER_FILES}
  ${OVIS_CORE_SOURCE_FILES}
)
target_include_directories(
  OvisCore
  PUBLIC
    ${CMAKE_SOURCE_DIR}/extern/SDL/include
)
target_link_libraries(
  OvisCore
  PUBLIC
    SDL2
    rapidjson
)
set_target_properties(
  OvisCore
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)