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
    PUBLIC cxx_std_20
)