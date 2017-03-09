option(SDL_SHARED "" off)
add_subdirectory(SDL)

add_subdirectory(freetype)

option(TB_BUILD_DEMO "" off)
add_subdirectory(turbobadger)
