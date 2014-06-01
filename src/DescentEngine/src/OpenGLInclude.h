#pragma once

#ifdef USE_SDL_OPENGL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#elif USE_ANDROID_OPENGL
#include <EGL/egl.h>
#include <GLES/gl.h>

#else
// fallback for Eclipse GUI
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#endif
