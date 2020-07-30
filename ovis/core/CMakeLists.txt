find_package(SDL2 REQUIRED)

add_library(
  OvisCore
  
  array_view.hpp
  class.hpp
  down_cast.hpp
  file.hpp file.cpp
  flags.hpp
  log.hpp
  range.hpp
  resource_manager.hpp resource_manager.cpp
  resource.hpp
)
target_include_directories(
  OvisCore
  PUBLIC
    ${OVIS_ROOT}
)
target_link_libraries(
  OvisCore
  PUBLIC
    SDL2::SDL2-static
    rapidjson
    fmt::fmt
)
target_compile_features(
  OvisCore
    PUBLIC cxx_std_20
)