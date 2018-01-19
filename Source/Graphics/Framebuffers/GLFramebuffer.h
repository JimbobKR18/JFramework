#ifndef __JFramework_GLFramebuffer_h_
#define __JFramework_GLFramebuffer_h_

#include "Common.h"
#include "Constants.h"
#include "GraphicsManager.h"
#include "Framebuffer.h"

#if defined(_WIN32)
  #include <GL\glew.h>
  #include "SDL.h"
#elif defined(__APPLE__)
  #include <GL/glew.h>
  #include <OpenGL/gl3.h>
  #include <OpenGL/gl3ext.h>
  #include <SDL2/SDL.h>
#else
  #include <GL/glew.h>
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
#endif

struct GLFramebuffer : public Framebuffer
{
  int           mWidth;
  int           mHeight;
  GLuint        mFramebufferProgramID;
  GLuint        mFrameBufferID;
  GLuint        mRenderedTextureID;
  GLuint        mVertexBufferID;
  GLuint        mTextureBufferID;
  GLuint        mIndexBufferID;
  GLuint        mVertexArrayObjectID;
  HashString    mVertexShaderFilename;
  HashString    mFragmentShaderFilename;
  std::vector<Vector3> mPositionCoords;
  std::vector<Vector2> mTexCoords;
  std::vector<unsigned> mIndices;
  
  GLFramebuffer(int aWidth, int aHeight);
  virtual ~GLFramebuffer();
  
  virtual void Generate(GraphicsManager *aManager);
  virtual void Bind();
  virtual void Draw(int aDefaultFramebuffer, int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen);
  
  void DisableVertexAttribArray(int aVertexAttrib);
  void PushRenderDataV2(std::vector<Vector2> &aData, int aAttribLocation, Vector2 const &aAttribute);
  void PushRenderDataV3(std::vector<Vector3> &aData, int aAttribLocation, Vector3 const &aAttribute);
  void PushRenderDataV4(std::vector<Vector4> &aData, int aAttribLocation, Vector4 const &aAttribute);
  void BindAttributeV2(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector2> &aData);
  void BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData);
  void BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData);
  void PrintGLError(int const aLineNumber);
};

#endif //__JFramework_GLFramebuffer_h_