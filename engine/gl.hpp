#pragma once

#include <SDL_platform.h>

#ifdef __IPHONEOS__
#include <SDL_opengles2.h>
#else
#include <OpenGL/gl3.h>
#endif
