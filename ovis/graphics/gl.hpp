#pragma once

#include <SDL_platform.h>

#if defined(__IPHONEOS__)
#include <SDL_opengles2.h>
#elif defined(__EMSCRIPTEN__)
#include <GLES2/gl2.h>
#else
#include <OpenGL/gl3.h>
#endif
