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
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(
  OvisCore
  PUBLIC
    ${SDL2_LIBRARIES}
    rapidjson
)
target_link_directories(
  OvisCore
  PUBLIC
    ${SDL2_LIBDIR}
)
set_target_properties(
  OvisCore
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)