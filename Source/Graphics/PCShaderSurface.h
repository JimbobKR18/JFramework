#ifndef __JFramework_PCShaderSurface_h_
#define __JFramework_PCShaderSurface_h_

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

#include "Surface.h"
#include "MathExt.h"

class PCShaderSurface : public Surface
{
private:
  GLuint        mTextureID;
  GLuint        mProgramID;
  GLuint        mVertexShaderID;
  GLuint        mFragmentShaderID;
  SDL_Surface*  mSurface;
  GLenum        mTextureFormat;
  GLint         mNumberOfColors;
  TTF_Font*     mFont;
  HashString    mVertexShaderFileName;
  HashString    mFragmentShaderFileName;
  
  static int const sUID;

public:
  PCShaderSurface();
  PCShaderSurface(GraphicsManager *aManager);
  virtual ~PCShaderSurface();

  virtual void        LoadImage(HashString const &aName);
  virtual Vector3     LoadText(HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth);
  virtual void        LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  
  // Getters
  unsigned            GetTextureID() const;
  unsigned            GetProgramID() const;
  unsigned            GetVertexShaderID() const;
  unsigned            GetFragmentShaderID() const;
  HashString const&   GetVertexShaderFilename() const;
  HashString const&   GetFragmentShaderFilename() const;

  // Derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(Parser &aParser);
  virtual void        Deserialize(Parser &aParser);
  static void         SerializeLUA();
  static int          GetUID() {return sUID;}
  
private:
  void                AddTexturePairing(HashString const &aName);
  void                PrintGLError(int const aLineNumber);
};

#endif
