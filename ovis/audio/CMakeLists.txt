add_library(
  OvisAudio

  audio_channel.hpp audio_channel.cpp
  audio_chunk.hpp audio_chunk.cpp
  audio_context.hpp audio_context.cpp
)

find_package(PkgConfig REQUIRED) 
pkg_check_modules(SDL2_mixer REQUIRED IMPORTED_TARGET SDL2_mixer)

target_link_libraries(
  OvisAudio
  PUBLIC
    OvisCore
    PkgConfig::SDL2_mixer
)
