#include "PCShaderScreen.h"
#include "PhysicsObject.h"
#include "PCShaderSurface.h"
#include "Constants.h"
#include "SurfaceProperty.h"
#include "GraphicsManager.h"

#define VERTEX_ARRAYS

#ifdef _WIN32
  #include <Windows.h>
  #include <gl/gl.h>
  #undef SendMessage
  #undef GetYValue
  #undef PlaySound
#endif

PCShaderScreen::PCShaderScreen() : Screen()
{
  assert(!"Do not use this");
}

PCShaderScreen::PCShaderScreen(int aW, int aH, bool aFullScreen) : Screen(aW, aH, aFullScreen), mWindow(nullptr), 
  mGLContext(), mDisplayMode(), mVertexArrayObjectID(0), mVertexBufferID(0), mTextureBufferID(0),
  mPositionBufferID(0), mColorBufferID(0), mIndexBufferID(0)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  
#if defined(__APPLE__)
  glewExperimental = GL_TRUE;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
  
  mWindow = SDL_CreateWindow(Constants::GetString("GameTitle").ToCharArray(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aW, aH,
                             SDL_WINDOW_OPENGL);
  mGLContext = SDL_GL_CreateContext(mWindow);
  SDL_GL_MakeCurrent(mWindow, mGLContext);
  SDL_GetDesktopDisplayMode(0, &mDisplayMode);
  
  GLenum glError = glewInit();
  if(glError != GLEW_OK)
  {
    DebugLogPrint("Error: %s\n", glewGetErrorString(glError));
    assert(!"GLEW failed to init.");
  }
  
  ChangeSize(aW, aH, aFullScreen);
}

PCShaderScreen::~PCShaderScreen()
{
  glDeleteBuffers(1, &mVertexBufferID);
  glDeleteBuffers(1, &mTextureBufferID);
  glDeleteBuffers(1, &mPositionBufferID);
  glDeleteBuffers(1, &mColorBufferID);
  glDeleteBuffers(1, &mIndexBufferID);
  glDeleteVertexArrays(1, &mVertexArrayObjectID);
  SDL_GL_DeleteContext(mGLContext);
  SDL_Quit();
}

/**
 * @brief Draw debug lines around objects, physics boxes only.
 * @param aObjects
 */
void PCShaderScreen::DebugDraw(std::vector<Surface*> const &aObjects)
{
  // Draw debug hitboxes for objects in environment, requires PhysicsObject
  Vector3 &cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetHalfSize();
  Vector3 cameraDiff = cameraPosition - cameraSize;
  
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end; ++it)
  {
    GameObject *obj = (*it)->GetOwner();

    if(obj->HAS<PhysicsObject>())
    {
      Transform *transform = obj->GET<Transform>();
      PhysicsObject *physicsObject = obj->GET<PhysicsObject>();
      Vector3 position = transform->GetPosition();
      Vector3 scale = transform->GetScale();
      Vector3 broadSize = physicsObject->GetBroadSize();

      if((*it)->GetViewMode() == VIEW_ABSOLUTE)
      {
        position -= cameraDiff;
      }

      glLoadIdentity();
      glPushMatrix();
      
      // Get the texture id of the surface
      PCShaderSurface *surface = obj->GET<PCShaderSurface>();
      GLuint program = surface->GetProgramID();
      
      // Start using shader
      glUseProgram(program);
      glUniform1i(glGetUniformLocation(program, "textureUnit"), 0);
      glUniform3f(glGetUniformLocation(program, "objectPos"), 0, 0, 0);
      glUniform4f(glGetUniformLocation(program, "primaryColor"), 0, 0, 0, 1);
      glUniform3f(glGetUniformLocation(program, "cameraDiff"), 0, 0, 0);

      // For each shape, draw the outline
      std::vector<Shape*> const& shapes = physicsObject->GetShapes();
      PhysicsObject::constShapeIT end = shapes.end();
      for(PhysicsObject::constShapeIT it = shapes.begin(); it != end; ++it)
      {
        if((*it)->shape == Shape::SPHERE)
        {
          Vector3 spherePos = position + (*it)->position.Multiply(scale);
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          
          // Rotate in a circle
          for(int i = 0; i < 360; ++i)
          {
            float radians = i * DEGREE_TO_RADS;
            float x = (*it)->GetSize(0) * cos(radians) * scale.x;
            float y = (*it)->GetSize(1) * sin(radians) * scale.y;
            glVertex3f(spherePos.x + x, spherePos.y + y, spherePos.z);
          }
          glEnd();
        }
        else if((*it)->shape == Shape::CUBE)
        {
          Vector3 cubePos = position + (*it)->position.Multiply(scale);
          float xSize = (*it)->GetSize(0) * scale.x;
          float ySize = (*it)->GetSize(1) * scale.y;
          
          // Physics Box Line
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          glVertex3f(cubePos.x - xSize, cubePos.y - ySize, cubePos.z);
          glVertex3f(cubePos.x + xSize, cubePos.y - ySize, cubePos.z);
          glVertex3f(cubePos.x + xSize, cubePos.y + ySize, cubePos.z);
          glVertex3f(cubePos.x - xSize, cubePos.y + ySize, cubePos.z);
          glVertex3f(cubePos.x - xSize, cubePos.y - ySize, cubePos.z);
          glEnd();
        }
        else if((*it)->shape == Shape::TRIANGLE)
        {
          Triangle* triangle = (Triangle*)(*it);
          Vector3 triPos = position + (*it)->position.Multiply(scale);
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          for(int i = 0; i < 3; ++i)
          {
            Vector3 point = triPos + triangle->GetPoint(i).Multiply(scale);
            glVertex3f(point.x, point.y, point.z);
          }
          Vector3 point = triPos + triangle->GetPoint(0).Multiply(scale);
          glVertex3f(point.x, point.y, point.z);
          glEnd();
        }
        else if((*it)->shape == Shape::LINE)
        {
          Line *line = (Line*)(*it);
          Vector3 linePos = position + (*it)->position.Multiply(scale);
          Vector3 lineEnd = linePos + line->direction.Multiply(scale) * line->length;
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          glVertex3f(linePos.x, linePos.y, linePos.z);
          glVertex3f(lineEnd.x, lineEnd.y, lineEnd.z);
          glEnd();
        }
      }

      // Broad Size Line
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0f, 1.0f, 0.0f);
      glVertex3f(position.x - broadSize.x, position.y - broadSize.y, position.z);
      glVertex3f(position.x + broadSize.x, position.y - broadSize.y, position.z);
      glVertex3f(position.x + broadSize.x, position.y + broadSize.y, position.z);
      glVertex3f(position.x - broadSize.x, position.y + broadSize.y, position.z);
      glVertex3f(position.x - broadSize.x, position.y - broadSize.y, position.z);
      glEnd();
      glPopMatrix();
    }
  }
}

/**
 * @brief Operations to run before draw phase.
 */
void PCShaderScreen::PreDraw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Draw objects
 * @param aObjects
 */
void PCShaderScreen::Draw(std::vector<Surface*> const &aObjects)
{
  // Camera position and size
  Vector3 &cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetHalfSize();
  Matrix33 viewMatrix = GetView().GetFinalTransform();
  Matrix33 identityMatrix = Matrix33();
  
  // Must scale, rotate, then translate camera offset
  Vector3 cameraDiff = (viewMatrix * cameraPosition) - cameraSize;
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end;)
  {
    // Get the texture id of the surface
    PCShaderSurface *surface = (*it)->GetOwner()->GET<PCShaderSurface>();
    GLuint texture = surface->GetTextureID();
    GLuint program = surface->GetProgramID();
    Viewspace viewSpace = surface->GetViewMode();
    Vector4 cameraTranslation;
    
    // Start using shader
    glUseProgram(program);
    int activeTexture = texture % GL_MAX_TEXTURE_UNITS;
    int vertexPosLocation = glGetAttribLocation(program, "vertexPos");
    int texCoordPosLocation = glGetAttribLocation(program, "texCoord");
    int objectPosLocation = glGetAttribLocation(program, "objectPos");
    int colorPosLocation = glGetAttribLocation(program, "primaryColor");
    
    // Camera translation
    float cameraMatrix[9];
    if(surface->GetViewMode() == VIEW_ABSOLUTE)
    {
      cameraTranslation = cameraDiff;
      
      for(int i = 0; i < 9; ++i)
      {
        cameraMatrix[i] = viewMatrix.values[i/3][i%3];
      }
    }
    else // Menu objects do not rotate.
    {
      for(int i = 0; i < 9; ++i)
      {
        cameraMatrix[i] = identityMatrix.values[i/3][i%3];
      }
    }
    
    std::vector<Vector4> vertexData, textureData, colorData, positionData;
    std::vector<GLuint> indices;
    
    vertexData.reserve(1024);
    textureData.reserve(1024);
    colorData.reserve(1024);
    positionData.reserve(1024);
    indices.reserve(1024);
    int iteration = 0;
    
    while(it != end && (*it)->GetProgramID() == program &&
          (*it)->GetTextureID() == texture && (*it)->GetViewMode() == viewSpace)
    {
      GameObject *owner = (*it)->GetOwner();
      PCShaderSurface *surface = owner->GET<PCShaderSurface>();
      Transform *transform = owner->GET<Transform>();
    
      // Get transforms in local and world space.
      Matrix33 modelTransform = transform->GetRotation() * Matrix33(transform->GetScale());
      Vector3 position = transform->GetPosition();
      TextureCoordinates *texCoord = surface->GetTextureData();
      Vector3 &size = transform->GetSize();
      Vector4 &color = surface->GetColor();

      // Move object based on its alignment
      AlignmentHelper(transform, size, position);
      
      // Get the basic coordinates for the quad
      Vector3 topLeft(-size.x, -size.y, 0);
      Vector3 topRight(size.x, -size.y, 0);
      Vector3 bottomRight(size.x, size.y, 0);
      Vector3 bottomLeft(-size.x, size.y, 0);
      
      // Model transform
      topLeft = modelTransform * topLeft;
      topRight = modelTransform * topRight;
      bottomLeft = modelTransform * bottomLeft;
      bottomRight = modelTransform * bottomRight;
      
      // Vertex points
      PushRenderData(vertexData, vertexPosLocation, topLeft);
      PushRenderData(vertexData, vertexPosLocation, bottomLeft);
      PushRenderData(vertexData, vertexPosLocation, bottomRight);
      PushRenderData(vertexData, vertexPosLocation, bottomRight);
      PushRenderData(vertexData, vertexPosLocation, topRight);
      PushRenderData(vertexData, vertexPosLocation, topLeft);
      
      PushRenderData(textureData, texCoordPosLocation, Vector4(texCoord->GetXValue(0), texCoord->GetYValue(0), 0, 1));
      PushRenderData(textureData, texCoordPosLocation, Vector4(texCoord->GetXValue(0), texCoord->GetYValue(1), 0, 1));
      PushRenderData(textureData, texCoordPosLocation, Vector4(texCoord->GetXValue(1), texCoord->GetYValue(1), 0, 1));
      PushRenderData(textureData, texCoordPosLocation, Vector4(texCoord->GetXValue(1), texCoord->GetYValue(1), 0, 1));
      PushRenderData(textureData, texCoordPosLocation, Vector4(texCoord->GetXValue(1), texCoord->GetYValue(0), 0, 1));
      PushRenderData(textureData, texCoordPosLocation, Vector4(texCoord->GetXValue(0), texCoord->GetYValue(0), 0, 1));
      
      PushRenderData(colorData, colorPosLocation, color);
      PushRenderData(colorData, colorPosLocation, color);
      PushRenderData(colorData, colorPosLocation, color);
      PushRenderData(colorData, colorPosLocation, color);
      PushRenderData(colorData, colorPosLocation, color);
      PushRenderData(colorData, colorPosLocation, color);
      
      PushRenderData(positionData, objectPosLocation, position);
      PushRenderData(positionData, objectPosLocation, position);
      PushRenderData(positionData, objectPosLocation, position);
      PushRenderData(positionData, objectPosLocation, position);
      PushRenderData(positionData, objectPosLocation, position);
      PushRenderData(positionData, objectPosLocation, position);
      
      for(int i = 0; i < 6; ++i)
      {
        indices.push_back(i + (iteration * 6));
      }
      
      if((*it)->GetProperties().size() != 0)
      {
        ++it;
        break;
      }
      else
      {
        ++it;
      }
      
      ++iteration;
    }
    
    // Enable textures and set uniforms.
    glBindVertexArray(mVertexArrayObjectID);
    glActiveTexture(GL_TEXTURE0 + activeTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "textureUnit"), activeTexture);
    glUniform3f(glGetUniformLocation(program, "cameraDiff"), cameraTranslation.x, cameraTranslation.y, cameraTranslation.z);
    glUniform3f(glGetUniformLocation(program, "cameraSize"), cameraSize.x, cameraSize.y, cameraSize.z);
    glUniformMatrix3fv(glGetUniformLocation(program, "cameraTransform"), 1, GL_TRUE, cameraMatrix);
    
    // Set shader properties. Due to batching, done on a per surface / shader basis.
    // Shader uniforms are reset upon relinking.
    SetShaderProperties(surface, true);
    
    // Set VBO and buffer data.
    EnableVertexAttribArray(vertexPosLocation);
    EnableVertexAttribArray(texCoordPosLocation);
    EnableVertexAttribArray(objectPosLocation);
    EnableVertexAttribArray(colorPosLocation);
    
    glBindVertexArray(mVertexArrayObjectID);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * vertexData.size(), &vertexData[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vertexPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    glBindBuffer(GL_ARRAY_BUFFER, mTextureBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * textureData.size(), &textureData[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(texCoordPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    glBindBuffer(GL_ARRAY_BUFFER, mColorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * colorData.size(), &colorData[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(colorPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * positionData.size(), &positionData[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(objectPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);
    
    // Draw and disable
    glDrawElements(GL_TRIANGLES, static_cast<unsigned>(vertexData.size()), GL_UNSIGNED_INT, 0);
    DisableVertexAttribArray(vertexPosLocation);
    DisableVertexAttribArray(texCoordPosLocation);
    DisableVertexAttribArray(objectPosLocation);
    DisableVertexAttribArray(colorPosLocation);
    
    // Reset shader property values.
    SetShaderProperties(surface, false);

    // Reset to default texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }
}

/**
 * @brief Draw UI objects
 * @param aObjects
 */
void PCShaderScreen::DrawUI(std::vector<Surface*> const &aObjects)
{
  Draw(aObjects);
}

/**
 * @brief Swap buffers
 */
void PCShaderScreen::SwapBuffers()
{
  SDL_GL_SwapWindow(mWindow);
}

/**
 * @brief Change window size
 * @param aW Width
 * @param aH Height
 * @param aFullScreen
 */
void PCShaderScreen::ChangeSize(int aW, int aH, bool aFullScreen)
{
  // Set full screen or not
  SetWidth(aW);
  SetHeight(aH);
  SetFullScreen(aFullScreen);
  int fullScreen = 0;
  if(aFullScreen)
    fullScreen = SDL_WINDOW_FULLSCREEN_DESKTOP;
  SDL_SetWindowFullscreen(mWindow, fullScreen);
  
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,        1);
  
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);

  if(aFullScreen)
    glViewport((mDisplayMode.w - aW)/2, (mDisplayMode.h - aH)/2, aW, aH);
  else
    glViewport(0, 0, aW, aH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, aW, aH, 0, 1, -1);

  glMatrixMode(GL_MODELVIEW);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);

  glLoadIdentity();
  
  glGenVertexArrays(1, &mVertexArrayObjectID);
  glGenBuffers(1, &mVertexBufferID);
  glGenBuffers(1, &mTextureBufferID);
  glGenBuffers(1, &mPositionBufferID);
  glGenBuffers(1, &mColorBufferID);
  glGenBuffers(1, &mIndexBufferID);
}

/**
 * @brief Align objects
 * @param aTransform Transform of object
 * @param aSize Object size
 * @param aPosition Object position
 */
void PCShaderScreen::AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition)
{
  X_ALIGNMENT xAlign = aTransform->GetXAlignment();
  Y_ALIGNMENT yAlign = aTransform->GetYAlignment();
  Z_ALIGNMENT zAlign = aTransform->GetZAlignment();

  switch(xAlign)
  {
  case X_ALIGN_LEFT:
    aPosition.x += aSize.x;
    break;
  case X_ALIGN_RIGHT:
    aPosition.x -= aSize.x;
    break;
  default:
    break;
  }

  switch(yAlign)
  {
  case Y_ALIGN_TOP:
    aPosition.y += aSize.y;
    break;
  case Y_ALIGN_BOTTOM:
    aPosition.y -= aSize.y;
    break;
  default:
    break;
  }

  switch(zAlign)
  {
  case Z_ALIGN_FRONT:
    aPosition.z += aSize.z;
    break;
  case Z_ALIGN_BACK:
    aPosition.z -= aSize.z;
    break;
  default:
    break;
  }
}

/**
 * @brief Check if point is on screen
 * @param aPoint Point to check
 */
bool PCShaderScreen::PointIsOnScreen(Vector3 const &aPoint)
{
  return aPoint.x < GetWidth() && aPoint.y < GetHeight() &&
         aPoint.x > 0 && aPoint.y > 0;
}

/**
 * @brief Check if box is on screen
 * @param aStart Top left of box
 * @param aEnd Bottom right of box
 */
bool PCShaderScreen::BoxIsOnScreen(Vector3 const &aStart, Vector3 const &aEnd)
{
  // Separating axis theorum
  Vector3 objectCenter = (aStart + aEnd) * 0.5f;
  Vector3 objectSize = (aEnd - aStart) * 0.5f;
  Vector3 screenCenter = Vector3(GetWidth() / 2.0f, GetHeight() / 2.0f, 0.0f);
  Vector3 screenSize = Vector3(GetWidth() / 2.0f, GetHeight() / 2.0f, 0.0f);
  for(int i = 0; i < 2; ++i)
  {
    if(objectCenter[i] - screenCenter[i] < objectSize[i] + screenSize[i])
    {
      continue;
    }
    return false;
  }
  return true;
}

/**
 * @brief Set shader properties based on surface.
 * @param aSurface Surface to set properties for.
 * @param aActive Decide whether or not to set values or reset.
 */
void PCShaderScreen::SetShaderProperties(PCShaderSurface *aSurface, bool aActive)
{
  // Set properties for shader. Separated by program id.
  GLuint program = aSurface->GetProgramID();
  Surface::PropertyContainer const &properties = aSurface->GetProperties();
  Surface::PropertyContainerConstIt propertyEnd = properties.end();
  for(Surface::PropertyContainerConstIt propertyIt = properties.begin(); propertyIt != propertyEnd; ++propertyIt)
  {
    SurfaceProperty *property = *propertyIt;
    HashString value = property->GetTargetValue();
    
    if(!aActive)
      value = property->GetDefaultValue();
    
    switch(property->GetType())
    {
      case PropertyType::INT1:
      {
        glUniform1i(glGetUniformLocation(program, property->GetName()), value.ToInt());
        break;
      }
      case PropertyType::INT3:
      {
        std::vector<int> intVector = value.ToIntVector();
        glUniform3i(glGetUniformLocation(program, property->GetName()), intVector[0], intVector[1], intVector[2]);
        break;
      }
      case PropertyType::INT4:
      {
        std::vector<int> intVector = value.ToIntVector();
        glUniform4i(glGetUniformLocation(program, property->GetName()), intVector[0], intVector[1], intVector[2], intVector[3]);
        break;
      }
      case PropertyType::FLOAT1:
      {
        glUniform1f(glGetUniformLocation(program, property->GetName()), value.ToFloat());
        break;
      }
      case PropertyType::FLOAT3:
      {
        std::vector<float> floatVector = value.ToFloatVector();
        glUniform3f(glGetUniformLocation(program, property->GetName()), floatVector[0], floatVector[1], floatVector[2]);
        break;
      }
      case PropertyType::FLOAT4:
      {
        std::vector<float> floatVector = value.ToFloatVector();
        glUniform4f(glGetUniformLocation(program, property->GetName()), floatVector[0], floatVector[1], floatVector[2], floatVector[3]);
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
 * @brief Enable attribute array
 * @param aVertexAttrib Index of attribute
 */
void PCShaderScreen::EnableVertexAttribArray(int aVertexAttrib)
{
  // -1 is error state
  if(aVertexAttrib > -1)
  {
    glEnableVertexAttribArray(aVertexAttrib);
  }
}

/**
 * @brief Disable attribute array
 * @param aVertexAttrib Index of attribute
 */
void PCShaderScreen::DisableVertexAttribArray(int aVertexAttrib)
{
  // -1 is error state
  if(aVertexAttrib > -1)
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
void PCShaderScreen::PushRenderData(std::vector<Vector4> &aData, int aAttribLocation, Vector4 const &aAttribute)
{
  // -1 is error state
  if(aAttribLocation > -1)
  {
    aData.push_back(aAttribute);
  }
}

/**
 * @brief Prints GL error info.
 * @param aLineNumber Use __LINE__ wherever you use this.
 */
void PCShaderScreen::PrintGLError(int const aLineNumber)
{
#ifndef _WIN32
  GLenum errorCode = glGetError();
  if(errorCode != 0)
    DebugLogPrint("(%s) (%i) %i: %s\n", __FILE__, aLineNumber, errorCode, gluErrorString(errorCode));
#endif
}
