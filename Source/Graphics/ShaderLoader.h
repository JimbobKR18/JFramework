#ifndef __JFramework_ShaderLoader_h_
#define __JFramework_ShaderLoader_h_

#include "Common.h"
#include "ShaderData.h"
#include "TextureData.h"
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
private:
  static std::vector<SDL_Surface*> mSurfaces;
  
public:
  ShaderLoader();
  virtual ~ShaderLoader();
  
  static ShaderData* LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  static TextureData* LoadTexture(HashString const &aTextureFileName, HashString const &aMinFilter, HashString const &aMagFilter);
  static TextureData* LoadText(HashString const &aFont, HashString const &aText, HashString const &aMinFilter, HashString const &aMagFilter, 
    Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth);
  static void Clear();
  
private:
  static int ImportTexture(SDL_Surface* aSurface, GLenum aTextureFormat, HashString const &aMinFilter, HashString const &aMagFilter);
};

#endif // __JFramework_ShaderLoader_h_
