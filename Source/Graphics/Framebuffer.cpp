#include "Framebuffer.h"
#include "ShaderLoader.h"
#include "GraphicsManager.h"

Framebuffer::Framebuffer(int aWidth, int aHeight) : mWidth(aWidth), mHeight(aHeight),
  mFramebufferProgramID(0), mFrameBufferID(0), mRenderedTextureID(0), mVertexBufferID(0), 
  mTextureBufferID(0), mIndexBufferID(0), mVertexArrayObjectID(0),
  mVertexShaderFilename(Constants::GetString("FramebufferVertexShaderFileName")), 
  mFragmentShaderFilename(Constants::GetString("FramebufferFragmentShaderFileName"))
{
}

Framebuffer::~Framebuffer()
{
  glDeleteBuffers(1, &mVertexBufferID);
  glDeleteBuffers(1, &mTextureBufferID);
  glDeleteBuffers(1, &mIndexBufferID);
  glDeleteVertexArrays(1, &mVertexArrayObjectID);
  glDeleteFramebuffers(1, &mFrameBufferID);
  glDeleteTextures(1, &mRenderedTextureID);
}

/**
 * @brief Generate framebuffer information.
 * @param aManager Cache shader stuff.
 */
void Framebuffer::Generate(GraphicsManager *aManager)
{
  glGenFramebuffers(1, &mFrameBufferID);
  glGenTextures(1, &mRenderedTextureID);
  glBindTexture(GL_TEXTURE_2D, mRenderedTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderedTextureID, 0);
  
  std::vector<HashString> shaderInfo = ShaderLoader::LoadShaders(mVertexShaderFilename, mFragmentShaderFilename);
  aManager->AddShaderPairing(mVertexShaderFilename + mFragmentShaderFilename, 
    ShaderData(shaderInfo[0].ToInt(), shaderInfo[1].ToInt(), shaderInfo[2].ToInt(), shaderInfo[3], shaderInfo[4]));
  mFramebufferProgramID = shaderInfo[0].ToInt();
  
  glGenBuffers(1, &mVertexBufferID);
  glGenBuffers(1, &mTextureBufferID);
  glGenBuffers(1, &mIndexBufferID);
  glGenVertexArrays(1, &mVertexArrayObjectID);
  
  mPositionCoords.push_back(Vector3(-1,-1,0));
  mPositionCoords.push_back(Vector3(1,-1,0));
  mPositionCoords.push_back(Vector3(1,1,0));
  mPositionCoords.push_back(Vector3(1,1,0));
  mPositionCoords.push_back(Vector3(-1,1,0));
  mPositionCoords.push_back(Vector3(-1,-1,0));
  
  mTexCoords.push_back(Vector2(0,0));
  mTexCoords.push_back(Vector2(1,0));
  mTexCoords.push_back(Vector2(1,1));
  mTexCoords.push_back(Vector2(1,1));
  mTexCoords.push_back(Vector2(0,1));
  mTexCoords.push_back(Vector2(0,0));
  
  mIndices.push_back(0);
  mIndices.push_back(1);
  mIndices.push_back(2);
  mIndices.push_back(3);
  mIndices.push_back(4);
  mIndices.push_back(5);
}

/**
 * @brief Bind framebuffer to be written to.
 */
void Framebuffer::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
  glViewport(0, 0, mWidth, mHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Draw elements from framebuffer to screen.
 * @param aDefaultFramebuffer Screen framebuffer.
 * @param aDefaultWidth Width of game screen.
 * @param aDefaultHeight Height of game screen.
 * @param aScreenWidth Literal width of window.
 * @param aScreenHeight Literal height of window.
 * @param aFullScreen Is this full screen?
 */
void Framebuffer::Draw(int aDefaultFramebuffer, int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen)
{
  int texCoordPosLocation = glGetAttribLocation(mFramebufferProgramID, "texCoord");
  int posCoordPosLocation = glGetAttribLocation(mFramebufferProgramID, "vertexPos");
  
  glBindFramebuffer(GL_FRAMEBUFFER, aDefaultFramebuffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  if(aFullScreen)
  {
    float ratio = ((float)aScreenHeight / (float)aDefaultHeight);
    int x = aDefaultWidth * ratio;
    int y = aDefaultHeight * ratio;
    glViewport((aScreenWidth - x)/2, (aScreenHeight - y)/2, aDefaultWidth * ratio, aScreenHeight);
  }
  else
    glViewport(0, 0, aDefaultWidth, aDefaultHeight);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glBindVertexArray(mVertexArrayObjectID);
  glUseProgram(mFramebufferProgramID);
  glUniform1i(glGetUniformLocation(mFramebufferProgramID, "textureUnit"), 0);
  BindAttributeV3(GL_ARRAY_BUFFER, mVertexBufferID, posCoordPosLocation, mPositionCoords);
  BindAttributeV2(GL_ARRAY_BUFFER, mTextureBufferID, texCoordPosLocation, mTexCoords);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mRenderedTextureID);
  glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
  
  DisableVertexAttribArray(posCoordPosLocation);
  DisableVertexAttribArray(texCoordPosLocation);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @brief Disable attribute array
 * @param aVertexAttrib Index of attribute
 */
void Framebuffer::DisableVertexAttribArray(int aVertexAttrib)
{
  // -1 is error state
  if(aVertexAttrib != -1)
  {
    glDisableVertexAttribArray(aVertexAttrib);
  }
}

/**
 * @brief Push render data onto vector if applicable.
 * @param aData Vector to push onto
 * @param aAttribLocation Location of attribute in shader
 * @param aAttribute Value of attribute
 */
void Framebuffer::PushRenderDataV2(std::vector<Vector2> &aData, int aAttribLocation, Vector2 const &aAttribute)
{
  // -1 is error state
  if(aAttribLocation != -1)
  {
    aData.push_back(aAttribute);
  }
}

/**
 * @brief Push render data onto vector if applicable.
 * @param aData Vector to push onto
 * @param aAttribLocation Location of attribute in shader
 * @param aAttribute Value of attribute
 */
void Framebuffer::PushRenderDataV3(std::vector<Vector3> &aData, int aAttribLocation, Vector3 const &aAttribute)
{
  // -1 is error state
  if(aAttribLocation != -1)
  {
    aData.push_back(aAttribute);
  }
}

/**
 * @brief Push render data onto vector if applicable.
 * @param aData Vector to push onto
 * @param aAttribLocation Location of attribute in shader
 * @param aAttribute Value of attribute
 */
void Framebuffer::PushRenderDataV4(std::vector<Vector4> &aData, int aAttribLocation, Vector4 const &aAttribute)
{
  // -1 is error state
  if(aAttribLocation != -1)
  {
    aData.push_back(aAttribute);
  }
}

/**
 * @brief Bind render data if applicable
 * @param aAttributeID Id of buffer
 * @param aAttribLocation Location of attribute in shader
 * @param aData Vector to bind from
 */
void Framebuffer::BindAttributeV2(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector2> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    glBindBuffer(aTarget, aBufferID);
    glBufferData(aTarget, sizeof(Vector2) * aData.size(), &aData[0], GL_STATIC_DRAW);
    glVertexAttribPointer(aAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);
    glBindBuffer(aTarget, 0);
  }
}

/**
 * @brief Bind render data if applicable
 * @param aAttributeID Id of buffer
 * @param aAttribLocation Location of attribute in shader
 * @param aData Vector to bind from
 */
void Framebuffer::BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    glBindBuffer(aTarget, aBufferID);
    glBufferData(aTarget, sizeof(Vector3) * aData.size(), &aData[0], GL_STATIC_DRAW);
    glVertexAttribPointer(aAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
    glBindBuffer(aTarget, 0);
  }
}

/**
 * @brief Bind render data if applicable
 * @param aAttributeID Id of buffer
 * @param aAttribLocation Location of attribute in shader
 * @param aData Vector to bind from
 */
void Framebuffer::BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    glBindBuffer(aTarget, aBufferID);
    glBufferData(aTarget, sizeof(Vector4) * aData.size(), &aData[0], GL_STATIC_DRAW);
    glVertexAttribPointer(aAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    glBindBuffer(aTarget, 0);
  }
}

/**
 * @brief Prints GL error info.
 * @param aLineNumber Use __LINE__ wherever you use this.
 */
void Framebuffer::PrintGLError(int const aLineNumber)
{
#ifndef _WIN32
  GLenum errorCode = glGetError();
  if(errorCode != 0)
    DebugLogPrint("(%s) (%i) %i: %s\n", __FILE__, aLineNumber, errorCode, gluErrorString(errorCode));
#endif
}
