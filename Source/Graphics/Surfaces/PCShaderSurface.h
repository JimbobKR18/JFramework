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

#include "Renderable.h"
#include "MathExt.h"

class PCShaderSurface : public Renderable
{
private:
  GLuint        mTextureID;
  GLuint        mProgramID;
  GLuint        mVertexShaderID;
  GLuint        mFragmentShaderID;
  HashString    mVertexShaderFileName;
  HashString    mFragmentShaderFileName;
  
  static int const sUID;

public:
  PCShaderSurface();
  PCShaderSurface(GraphicsManager *aManager);
  PCShaderSurface(PCShaderSurface const &aPCShaderSurface);
  virtual ~PCShaderSurface();

  virtual void        LoadImage(HashString const &aName);
  virtual void        LoadText(HashString const &aText);
  virtual void        LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  
  // Getters
  virtual unsigned    GetTextureID() const;
  virtual unsigned    GetProgramID() const;
  unsigned            GetVertexShaderID() const;
  unsigned            GetFragmentShaderID() const;
  HashString const&   GetVertexShaderFilename() const;
  HashString const&   GetFragmentShaderFilename() const;
  
  // Setters
  void                SetTextureID(unsigned const aTextureID);
  void                SetProgramID(unsigned const aProgramID);
  void                SetVertexShaderID(unsigned const aVertexShaderID);
  void                SetFragmentShaderID(unsigned const aFragmentShaderID);
  
  // Virtuals
  virtual VertexContainer GetVertexData(Vector3 const &aPosition) const;

  // Derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(ParserNode *aNode);
  virtual void        Deserialize(ParserNode *aNode);
  virtual Component*  Clone(GameObject *aNewOwner) const;
  static void         SerializeLUA();
  static int          GetUID() {return sUID;}
};

#endif
