#include "GLFramebuffer.h"
#include "ShaderLoader.h"
#include "GraphicsManager.h"
#include "SystemProperties.h"

GLFramebuffer::GLFramebuffer(int aWidth, int aHeight) : Framebuffer(), mWidth(aWidth), mHeight(aHeight),
  mFramebufferProgramID(0), mFrameBufferID(0), mRenderedTextureID(0), mVertexBufferID(0), 
  mTextureBufferID(0), mIndexBufferID(0), mVertexArrayObjectID(0),
  mVertexShaderFilename(SystemProperties::GetFramebufferVertexShaderName()), 
  mFragmentShaderFilename(SystemProperties::GetFramebufferFragmentShaderName())
{
}

GLFramebuffer::~GLFramebuffer()
{
  // TODO do NOT delete this program.
  //glDeleteProgram(mFramebufferProgramID);
  glDeleteBuffers(1, &mVertexBufferID);
  glDeleteBuffers(1, &mTextureBufferID);
  glDeleteBuffers(1, &mIndexBufferID);
  glDeleteVertexArrays(1, &mVertexArrayObjectID);
  glDeleteFramebuffers(1, &mFrameBufferID);
  glDeleteTextures(1, &mRenderedTextureID);
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
  GLint minFilter = GL_LINEAR;
  GLint magFilter = GL_LINEAR;
  if(SystemProperties::GetMinFilter() == "GL_NEAREST")
  {
    minFilter = GL_NEAREST;
  }
  else if(SystemProperties::GetMinFilter() == "GL_NEAREST_MIPMAP_NEAREST")
  {
    minFilter = GL_NEAREST_MIPMAP_NEAREST;
  }
  else if(SystemProperties::GetMinFilter() == "GL_NEAREST_MIPMAP_LINEAR")
  {
    minFilter = GL_NEAREST_MIPMAP_LINEAR;
  }
  else if(SystemProperties::GetMinFilter() == "GL_LINEAR_MIPMAP_NEAREST")
  {
    minFilter = GL_LINEAR_MIPMAP_NEAREST;
  }
  else if(SystemProperties::GetMinFilter() == "GL_LINEAR_MIPMAP_LINEAR")
  {
    minFilter = GL_LINEAR_MIPMAP_LINEAR;
  }
  if(SystemProperties::GetMagFilter() == "GL_NEAREST")
  {
    magFilter = GL_NEAREST;
  }
  
  //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glGenFramebuffers(1, &mFrameBufferID);
  glGenTextures(1, &mRenderedTextureID);
  glBindTexture(GL_TEXTURE_2D, mRenderedTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  SetShaders(aManager, mVertexShaderFilename, mFragmentShaderFilename);
  
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
void GLFramebuffer::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderedTextureID, 0);
  glViewport(0, 0, mWidth, mHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Unbind framebuffer from being written to.
 * @param aDefaultFramebuffer Screen framebuffer.
 */
void GLFramebuffer::Unbind(int aDefaultFramebuffer)
{
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindFramebuffer(GL_FRAMEBUFFER, aDefaultFramebuffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Draw elements from framebuffer to screen.
 * @param aDefaultWidth Width of game screen.
 * @param aDefaultHeight Height of game screen.
 * @param aScreenWidth Literal width of window.
 * @param aScreenHeight Literal height of window.
 * @param aFullScreen Is this full screen?
 */
void GLFramebuffer::Draw(int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen)
{
  int texCoordPosLocation = glGetAttribLocation(mFramebufferProgramID, "texCoord");
  int posCoordPosLocation = glGetAttribLocation(mFramebufferProgramID, "vertexPos");
  
  if(aFullScreen)
  {
    float ratio = ((float)aScreenHeight / (float)aDefaultHeight);
    int x = aDefaultWidth * ratio;
    int y = aDefaultHeight * ratio;
    glViewport((aScreenWidth - x)/2, (aScreenHeight - y)/2, aDefaultWidth * ratio, aScreenHeight);
  }
  else
    glViewport(0, 0, aDefaultWidth, aDefaultHeight);
  
  glBindVertexArray(mVertexArrayObjectID);
  glUseProgram(mFramebufferProgramID);
  glUniform1i(glGetUniformLocation(mFramebufferProgramID, "textureUnit"), 0);
  SetShaderProperties(true);
  BindAttributeV3(GL_ARRAY_BUFFER, mVertexBufferID, posCoordPosLocation, mPositionCoords);
  BindAttributeV2(GL_ARRAY_BUFFER, mTextureBufferID, texCoordPosLocation, mTexCoords);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mRenderedTextureID);
  glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
  
  DisableVertexAttribArray(posCoordPosLocation);
  DisableVertexAttribArray(texCoordPosLocation);
  SetShaderProperties(false);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    SurfaceProperty *property = *propertyIt;
    HashString value = property->GetTargetValue();
    
    if(!aActive)
      value = property->GetDefaultValue();
      
    #ifdef _DEBUG_DRAW
      //DebugLogPrint("Setting uniform %s to %s\n", property->GetName().ToCharArray(), value.ToCharArray());
    #endif
    
    switch(property->GetType())
    {
      case PropertyType::INT1:
      {
        glUniform1i(glGetUniformLocation(mFramebufferProgramID, property->GetName()), value.ToInt());
        break;
      }
      case PropertyType::INT3:
      {
        std::vector<int> intVector = value.ToIntVector();
        glUniform3i(glGetUniformLocation(mFramebufferProgramID, property->GetName()), intVector[0], intVector[1], intVector[2]);
        break;
      }
      case PropertyType::INT4:
      {
        std::vector<int> intVector = value.ToIntVector();
        glUniform4i(glGetUniformLocation(mFramebufferProgramID, property->GetName()), intVector[0], intVector[1], intVector[2], intVector[3]);
        break;
      }
      case PropertyType::FLOAT1:
      {
        glUniform1f(glGetUniformLocation(mFramebufferProgramID, property->GetName()), value.ToFloat());
        break;
      }
      case PropertyType::FLOAT3:
      {
        std::vector<float> floatVector = value.ToFloatVector();
        glUniform3f(glGetUniformLocation(mFramebufferProgramID, property->GetName()), floatVector[0], floatVector[1], floatVector[2]);
        break;
      }
      case PropertyType::FLOAT4:
      {
        std::vector<float> floatVector = value.ToFloatVector();
        glUniform4f(glGetUniformLocation(mFramebufferProgramID, property->GetName()), floatVector[0], floatVector[1], floatVector[2], floatVector[3]);
        break;
      }
      default:
      {
        assert(!"Invalid property type.");
        break;
      }
    }
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
void GLFramebuffer::BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData)
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
void GLFramebuffer::BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData)
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
void GLFramebuffer::PrintGLError(int const aLineNumber)
{
#ifndef _WIN32
  GLenum errorCode = glGetError();
  if(errorCode != 0)
    DebugLogPrint("(%s) (%i) %i: %s\n", __FILE__, aLineNumber, errorCode, gluErrorString(errorCode));
#endif
}
