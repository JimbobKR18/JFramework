#ifndef __JFramework_PCSurface_h_
#define __JFramework_PCSurface_h_

#if defined(_WIN32)
  #include "SDL.h"
  #include "SDL_ttf.h"
  typedef int GLint;
  typedef unsigned int GLuint;
  typedef unsigned int GLenum;
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <SDL2/SDL.h>
  #include <SDL2_ttf/SDL_ttf.h>
#else
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_ttf.h>
#endif

#include "Surface.h"
#include "MathExt.h"

class PCSurface : public Surface
{
private:
  GLuint        mTextureID;
  SDL_Surface*  mSurface;
  GLenum        mTextureFormat;
  GLint         mNumberOfColors;
  TTF_Font*     mFont;
  
  static int const sUID;

public:
  PCSurface();
  PCSurface(GraphicsManager *aManager);
  virtual ~PCSurface();

  virtual void        LoadImage(HashString const &aName);
  virtual Vector3     LoadText(HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth);
  virtual void        LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  
  // Getters
  unsigned            GetTextureID() const;

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(Parser &aParser);
  virtual void        Deserialize(Parser &aParser);
  static void         SerializeLUA();
  static int          GetUID() {return sUID;}
  
private:
  void                AddTexturePairing(HashString const &aName);
};

#endif
