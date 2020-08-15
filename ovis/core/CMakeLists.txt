
add_library(
  OvisCore
  
  array_view.hpp
  class.hpp
  down_cast.hpp
  file.hpp file.cpp
  flags.hpp
  log.hpp log.cpp
  range.hpp
  resource_manager.hpp resource_manager.cpp
  resource.hpp
  profiling.hpp profiling.cpp
  )

target_include_directories(
  OvisCore
  PUBLIC
    ${OVIS_ROOT}
)

target_link_libraries(
  OvisCore
  PUBLIC
    rapidjson
    fmt::fmt
)

if (OVIS_EMSCRIPTEN)
  target_compile_options(
    OvisCore
    PUBLIC
      -std=c++1z
      -g
      "SHELL:-s USE_SDL=2"
      "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
      "SHELL:-s DEMANGLE_SUPPORT=1"
  )
  target_link_options(
    OvisCore
    PUBLIC
      -g
      "SHELL:-s USE_SDL=2"
      "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
      "SHELL:-s DEMANGLE_SUPPORT=1"
  )
else ()
  find_package(SDL2 REQUIRED)
  target_link_libraries(
    OvisCore
    PUBLIC
      SDL2::SDL2-static
  )
endif ()

target_compile_features(
  OvisCore
  PUBLIC
    cxx_std_20
)
target_compile_definitions(
  OvisCore
  PUBLIC
    -DOVIS_ENABLE_BUILT_IN_PROFILING=1
)