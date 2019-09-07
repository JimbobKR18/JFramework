#ifndef __JFramework_ShaderLoader_h_
#define __JFramework_ShaderLoader_h_

#include "Common.h"
#include "ShaderData.h"
#include "TextureData.h"
#include "RenderableProperty.h"
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

#ifdef _DEBUG
  #define GL_ERROR_CHECK() ShaderLoader::LogError(__FILE__, __LINE__)
#else
  #define GL_ERROR_CHECK()
#endif

class GLTextureInfo
{
private:
  static HashString const GL_NEAREST_STRING;
  static HashString const GL_NEAREST_MIPMAP_NEAREST_STRING;
  static HashString const GL_NEAREST_MIPMAP_LINEAR_STRING;
  static HashString const GL_LINEAR_MIPMAP_NEAREST_STRING;
  static HashString const GL_LINEAR_MIPMAP_LINEAR_STRING;
  static HashString const GL_CLAMP_TO_EDGE_STRING;
  static HashString const GL_CLAMP_TO_BORDER_STRING;

public:
  GLuint mTextureID;
  GLint mMinFilter;
  GLint mMagFilter;
  GLint mWrapS;
  GLint mWrapT;
  
  GLTextureInfo();
  GLTextureInfo(HashString const &aMinFilter, HashString const &aMagFilter);
  void Set(HashString const &aMinFilter, HashString const &aMagFilter);
};

class ShaderLoader
{
private:
  static std::vector<SDL_Surface*> mSurfaces;
  
public:
  ShaderLoader();
  virtual ~ShaderLoader();
  
  static void LogError(HashString const &aFile, int aLine);
  static ShaderData* LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  static TextureData* LoadTexture(HashString const &aTextureFileName, HashString const &aMinFilter, HashString const &aMagFilter);
  static TextureData* LoadText(HashString const &aFont, HashString const &aText, HashString const &aMinFilter, HashString const &aMagFilter, 
    Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth);
  static void SetShaderUniform(int aProgram, HashString const &aName, PropertyType const &aPropertyType, HashString const &aValue, HashString const &aId);
  static void Clear();
  
private:
  static unsigned ImportTexture(SDL_Surface* aSurface, GLenum aTextureFormat, HashString const &aMinFilter, HashString const &aMagFilter);
};

#endif // __JFramework_ShaderLoader_h_
