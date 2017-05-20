#ifndef __JFramework_ShaderLoader_h_
#define __JFramework_ShaderLoader_h_

#include "Common.h"
#if defined(_WIN32)
  #include "SDL.h"
  #include "SDL_ttf.h"
  typedef int GLint;
  typedef unsigned int GLuint;
  typedef unsigned int GLenum;
#elif defined(__APPLE__)
  #include <GL/glew.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <SDL2/SDL.h>
  #include <SDL2_ttf/SDL_ttf.h>
#else
  #include <GL/glew.h>
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
#endif

class ShaderLoader
{
public:
  ShaderLoader();
  virtual ~ShaderLoader();
  
  static std::vector<HashString> LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
};

#endif // __JFramework_ShaderLoader_h_
