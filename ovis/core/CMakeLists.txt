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
    fmt::fmt
)
target_link_directories(
  OvisCore
  PUBLIC
    ${SDL2_LIBDIR}
)
target_compile_features(
  OvisCore
    PUBLIC cxx_std_20)