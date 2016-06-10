#ifndef __JFramework_PlatformIncludes_H_
#define __JFramework_PlatformIncludes_H_

#if defined(_WIN32)
  #include "SDL.h"
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <SDL2/SDL.h>
#else
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
#endif

#endif