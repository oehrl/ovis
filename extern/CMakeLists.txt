add_subdirectory(freetype)

if (NOT ${OVIS_EMSCRIPTEN})
  add_subdirectory(SDL)
endif ()