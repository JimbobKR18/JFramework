#ifndef __JFramework_GLFramebuffer_h_
#define __JFramework_GLFramebuffer_h_

#include "Common.h"
#include "Constants.h"
#include "GraphicsManager.h"
#include "Framebuffer.h"
#include "PlatformIncludes.h"

class GLFramebuffer : public Framebuffer
{
private:
  int           mWidth;
  int           mHeight;
  GLuint        mFramebufferProgramID;
  GLuint        mFrameBufferID;
  GLuint        mRenderedTextureID;
  GLuint        mDepthTextureID;
  GLuint        mVertexBufferID;
  GLuint        mTextureBufferID;
  GLuint        mIndexBufferID;
  GLuint        mVertexArrayObjectID;
  HashString    mVertexShaderFilename;
  HashString    mFragmentShaderFilename;
  HashString    mMinFilter;
  HashString    mMagFilter;
  std::vector<Vector3> mPositionCoords;
  std::vector<Vector2> mTexCoords;
  std::vector<unsigned> mIndices;
  bool          mGenerated;
  
public:
  GLFramebuffer(int aWidth, int aHeight, HashString const &aMinFilter, HashString const &aMagFilter);
  virtual ~GLFramebuffer();
  
  // VIRTUALS
  virtual int GetTextureID() const;
  virtual void SetShaders(GraphicsManager *aManager, HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  virtual void Generate(GraphicsManager *aManager);
  virtual void Bind();
  virtual void Unbind(int aDefaultFramebuffer);
  virtual void Draw(int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen);
  virtual void SetShaderProperties(bool aActive);
  
  // METHODS
  void DisableVertexAttribArray(int aVertexAttrib);
  void PushRenderDataV2(std::vector<Vector2> &aData, int aAttribLocation, Vector2 const &aAttribute);
  void PushRenderDataV3(std::vector<Vector3> &aData, int aAttribLocation, Vector3 const &aAttribute);
  void PushRenderDataV4(std::vector<Vector4> &aData, int aAttribLocation, Vector4 const &aAttribute);
  void BindAttributeV2(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector2> &aData);
  void BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData);
  void BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData);
};

#endif //__JFramework_GLFramebuffer_h_
