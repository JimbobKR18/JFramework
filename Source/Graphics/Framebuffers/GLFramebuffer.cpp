#include "GLFramebuffer.h"
#include "ShaderLoader.h"
#include "GraphicsManager.h"
#include "SystemProperties.h"

GLFramebuffer::GLFramebuffer(int aWidth, int aHeight, HashString const &aMinFilter, HashString const &aMagFilter) : Framebuffer(), 
  mWidth(aWidth), mHeight(aHeight), mFramebufferProgramID(0), mFrameBufferID(0), mRenderedTextureID(0), mDepthTextureID(0),
  mVertexBufferID(0), mTextureBufferID(0), mIndexBufferID(0), mVertexArrayObjectID(0),
  mVertexShaderFilename(SystemProperties::GetFramebufferVertexShaderName()), 
  mFragmentShaderFilename(SystemProperties::GetFramebufferFragmentShaderName()),
  mMinFilter(aMinFilter), mMagFilter(aMagFilter),
  mPositionCoords(), mTexCoords(), mIndices(), mGenerated(false)
{
}

GLFramebuffer::~GLFramebuffer()
{
  // TODO do NOT delete this program.
  //glDeleteProgram(mFramebufferProgramID);
  if(mGenerated)
  {
    glDeleteBuffers(1, &mVertexBufferID);
    glDeleteBuffers(1, &mTextureBufferID);
    glDeleteBuffers(1, &mIndexBufferID);
    glDeleteVertexArrays(1, &mVertexArrayObjectID);
    glDeleteFramebuffers(1, &mFrameBufferID);
    glDeleteTextures(1, &mRenderedTextureID);
    glDeleteTextures(1, &mDepthTextureID);
    
    mVertexBufferID = 0;
    mTextureBufferID = 0;
    mIndexBufferID = 0;
    mVertexArrayObjectID = 0;
    mFrameBufferID = 0;
    mRenderedTextureID = 0;
    mDepthTextureID = 0;
  }
}

/**
 * @brief Get rendered texture id.
 * @return Rendered texture id.
 */
int GLFramebuffer::GetTextureID() const
{
  return mRenderedTextureID;
}

/**
 * @brief Set shaders.
 * @param aManager Graphics manager for shader management.
 * @param aVertexShaderFilename Vertex shader name.
 * @param aFragmentShaderFilename Fragment shader name.
 */
void GLFramebuffer::SetShaders(GraphicsManager *aManager, HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename)
{
  mVertexShaderFilename = aVertexShaderFilename;
  mFragmentShaderFilename = aFragmentShaderFilename;
  
  HashString shaderKey = mVertexShaderFilename + mFragmentShaderFilename;
  if(aManager->ShaderDataExists(shaderKey))
  {
    ShaderData* shaderInfo = aManager->GetShaderData(shaderKey);
    mFramebufferProgramID = shaderInfo->mProgramID;
  }
  else
  {
    ShaderData* shaderInfo = ShaderLoader::LoadShaders(mVertexShaderFilename, mFragmentShaderFilename);
    aManager->AddShaderPairing(shaderKey, shaderInfo);
    mFramebufferProgramID = shaderInfo->mProgramID;
  }
}

/**
 * @brief Generate framebuffer information.
 * @param aManager Cache shader stuff.
 */
void GLFramebuffer::Generate(GraphicsManager *aManager)
{
  mGenerated = true;
  GLTextureInfo info(mMinFilter, mMagFilter);
  GLint minFilter = info.mMinFilter;
  GLint magFilter = info.mMagFilter;
  
  glGenFramebuffers(1, &mFrameBufferID);
  GL_ERROR_CHECK();
  glGenTextures(1, &mRenderedTextureID);
  GL_ERROR_CHECK();
  glGenTextures(1, &mDepthTextureID);
  GL_ERROR_CHECK();
  
  // Color Texture
  glActiveTexture(GL_TEXTURE0);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, mRenderedTextureID);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();
#ifdef __APPLE__
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE);
  GL_ERROR_CHECK();
  glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_FALSE);
  GL_ERROR_CHECK();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, 0);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
#endif
  GL_ERROR_CHECK();
  glGenerateMipmap(GL_TEXTURE_2D);
  GL_ERROR_CHECK();
  
  // Depth Texture
  glActiveTexture(GL_TEXTURE1);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  GL_ERROR_CHECK();
  glGenerateMipmap(GL_TEXTURE_2D);
  GL_ERROR_CHECK();
  
  SetShaders(aManager, mVertexShaderFilename, mFragmentShaderFilename);
  
  glGenVertexArrays(1, &mVertexArrayObjectID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mVertexBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mTextureBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mIndexBufferID);
  GL_ERROR_CHECK();
  
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
void GLFramebuffer::Bind()
{
  Vector4 const &clearColor = GetClearColor();
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
  GL_ERROR_CHECK();
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderedTextureID, 0);
  GL_ERROR_CHECK();
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTextureID, 0);
  GL_ERROR_CHECK();
  glViewport(0, 0, mWidth, mHeight);
  GL_ERROR_CHECK();
  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
  GL_ERROR_CHECK();
  glClearDepth(0.0f);
  GL_ERROR_CHECK();
  glDepthMask(GL_TRUE);
  GL_ERROR_CHECK();
  glDepthFunc(GL_GREATER);
  GL_ERROR_CHECK();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ERROR_CHECK();
}

/**
 * @brief Unbind framebuffer from being written to.
 * @param aDefaultFramebuffer Screen framebuffer.
 */
void GLFramebuffer::Unbind(int aDefaultFramebuffer)
{
  glGenerateMipmap(GL_TEXTURE_2D);
  GL_ERROR_CHECK();
  glBindFramebuffer(GL_FRAMEBUFFER, aDefaultFramebuffer);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, 0);
  GL_ERROR_CHECK();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ERROR_CHECK();
}

/**
 * @brief Draw elements from framebuffer to screen.
 * @param aDefaultWidth Width of game screen.
 * @param aDefaultHeight Height of game screen.
 * @param aScreenWidth Literal width of window.
 * @param aScreenHeight Literal height of window.
 * @param aFullScreen Is this full screen?
 */
void GLFramebuffer::Draw(GraphicsManager *aManager, int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen)
{
  int texCoordPosLocation = glGetAttribLocation(mFramebufferProgramID, "texCoord");
  int posCoordPosLocation = glGetAttribLocation(mFramebufferProgramID, "vertexPos");
  int x = aDefaultWidth;
  int y = aDefaultHeight;
  
  if(aFullScreen)
  {
    float ratio = ((float)aScreenHeight / (float)aDefaultHeight);
    x = aDefaultWidth * ratio;
    y = aDefaultHeight * ratio;
    if(x <= aScreenWidth)
    {
      glViewport((aScreenWidth - x)/2, (aScreenHeight - y)/2, x, aScreenHeight);
      y = aScreenHeight;
      GL_ERROR_CHECK();
    }
    else
    {
      ratio = ((float)aScreenWidth / (float)aDefaultWidth);
      x = aDefaultWidth * ratio;
      y = aDefaultHeight * ratio;
      glViewport((aScreenWidth - x)/2, (aScreenHeight - y)/2, aScreenWidth, y);
      x = aScreenWidth;
      GL_ERROR_CHECK();
    }
  }
  else
  {
    glViewport(0, 0, aDefaultWidth, aDefaultHeight);
    GL_ERROR_CHECK();
  }
  
  glBindVertexArray(mVertexArrayObjectID);
  GL_ERROR_CHECK();
  glUseProgram(mFramebufferProgramID);
  GL_ERROR_CHECK();
  glUniform1i(glGetUniformLocation(mFramebufferProgramID, "colorTextureUnit"), 0);
  GL_ERROR_CHECK();
  glUniform1i(glGetUniformLocation(mFramebufferProgramID, "depthTextureUnit"), 1);
  GL_ERROR_CHECK();
  
  // Optionals
  GLint location = -1;
  location = glGetUniformLocation(mFramebufferProgramID, "textureWidth");
  GL_ERROR_CHECK();
  if(location != -1)
  {
    glUniform1i(location, static_cast<int>(x));
    GL_ERROR_CHECK();
  }
  location = glGetUniformLocation(mFramebufferProgramID, "textureHeight");
  GL_ERROR_CHECK();
  if(location != -1)
  {
    glUniform1i(location, static_cast<int>(y));
    GL_ERROR_CHECK();
  }
  location = glGetUniformLocation(mFramebufferProgramID, "time");
  GL_ERROR_CHECK();
  if(location != -1)
  {
    glUniform1f(location, aManager->GetOwningApp()->GetAppTime());
    GL_ERROR_CHECK();
  }
  location = glGetUniformLocation(mFramebufferProgramID, "resolution");
  GL_ERROR_CHECK();
  if(location != -1)
  {
    glUniform2f(location, x, y);
    GL_ERROR_CHECK();
  }
  
  std::vector<int> const &inputs = GetInputTextures();
  int i = 2;
  for(std::vector<int>::const_iterator it = inputs.begin(); it != inputs.end(); ++it, i++)
  {
    HashString name = HashString("inputTextureUnit") + Common::IntToString(i - 1);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, *it);
    glUniform1i(glGetUniformLocation(mFramebufferProgramID, name.ToCharArray()), i);
    GL_ERROR_CHECK();
  }
  
  SetShaderProperties(true);
  BindAttributeV3(GL_ARRAY_BUFFER, mVertexBufferID, posCoordPosLocation, mPositionCoords);
  BindAttributeV2(GL_ARRAY_BUFFER, mTextureBufferID, texCoordPosLocation, mTexCoords);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
  GL_ERROR_CHECK();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
  GL_ERROR_CHECK();
  
  // Bind rendered texture
  glActiveTexture(GL_TEXTURE0);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, mRenderedTextureID);
  
  // Bind depth texture
  glActiveTexture(GL_TEXTURE1);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
  glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
  
  DisableVertexAttribArray(posCoordPosLocation);
  DisableVertexAttribArray(texCoordPosLocation);
  SetShaderProperties(false);
}

/**
 * @brief Set shader properties for framebuffer. HINT: If you're not seeing your intended results, remember
 *        to declare the field as uniform in your shader.
 * @param aActive Decide whether or not to set values or reset.
 */
void GLFramebuffer::SetShaderProperties(bool aActive)
{
  // Set properties for shader. Separated by program id.
  PropertyContainer const &properties = GetProperties();
  PropertyContainerConstIt propertyEnd = properties.end();
  for(PropertyContainerConstIt propertyIt = properties.begin(); propertyIt != propertyEnd; ++propertyIt)
  {
    RenderableProperty *property = propertyIt->second;
    HashString value = property->GetTargetValue();
    
    if(!aActive)
      value = property->GetDefaultValue();
      
    ShaderLoader::SetShaderUniform(mFramebufferProgramID, property->GetName(), property->GetType(), value, property->GetId());
  }
}

/**
 * @brief Disable attribute array
 * @param aVertexAttrib Index of attribute
 */
void GLFramebuffer::DisableVertexAttribArray(int aVertexAttrib)
{
  // -1 is error state
  if(aVertexAttrib != -1)
  {
    glDisableVertexAttribArray(aVertexAttrib);
    GL_ERROR_CHECK();
  }
}

/**
 * @brief Push render data onto vector if applicable.
 * @param aData Vector to push onto
 * @param aAttribLocation Location of attribute in shader
 * @param aAttribute Value of attribute
 */
void GLFramebuffer::PushRenderDataV2(std::vector<Vector2> &aData, int aAttribLocation, Vector2 const &aAttribute)
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
void GLFramebuffer::PushRenderDataV3(std::vector<Vector3> &aData, int aAttribLocation, Vector3 const &aAttribute)
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
void GLFramebuffer::PushRenderDataV4(std::vector<Vector4> &aData, int aAttribLocation, Vector4 const &aAttribute)
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
void GLFramebuffer::BindAttributeV2(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector2> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, aBufferID);
    GL_ERROR_CHECK();
    glBufferData(aTarget, sizeof(Vector2) * aData.size(), &aData[0], GL_STATIC_DRAW);
    GL_ERROR_CHECK();
    glVertexAttribPointer(aAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, 0);
    GL_ERROR_CHECK();
  }
}

/**
 * @brief Bind render data if applicable
 * @param aAttributeID Id of buffer
 * @param aAttribLocation Location of attribute in shader
 * @param aData Vector to bind from
 */
void GLFramebuffer::BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, aBufferID);
    GL_ERROR_CHECK();
    glBufferData(aTarget, sizeof(Vector3) * aData.size(), &aData[0], GL_STATIC_DRAW);
    GL_ERROR_CHECK();
    glVertexAttribPointer(aAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, 0);
    GL_ERROR_CHECK();
  }
}

/**
 * @brief Bind render data if applicable
 * @param aAttributeID Id of buffer
 * @param aAttribLocation Location of attribute in shader
 * @param aData Vector to bind from
 */
void GLFramebuffer::BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, aBufferID);
    GL_ERROR_CHECK();
    glBufferData(aTarget, sizeof(Vector4) * aData.size(), &aData[0], GL_STATIC_DRAW);
    GL_ERROR_CHECK();
    glVertexAttribPointer(aAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, 0);
    GL_ERROR_CHECK();
  }
}
