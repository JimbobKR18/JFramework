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
  mGLContext(), mDisplayMode(), mVertexBufferID(0), mVertexArrayObjectID(0)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  
//#if defined(__APPLE__)
  //glewExperimental = GL_TRUE;
  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//#endif
  
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
      
      // Get the texture id of the surface
      PCShaderSurface *surface = obj->GET<PCShaderSurface>();
      GLuint program = surface->GetProgramID();
      
      // Start using shader
      glUseProgram(program);

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
  glLoadIdentity();
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
  
  // Render data array.
  std::vector<Vector4> renderData;
  renderData.reserve(8);
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end; ++it)
  {
    // Get the texture id of the surface
    PCShaderSurface *surface = (*it)->GetOwner()->GET<PCShaderSurface>();
    GLuint texture = surface->GetTextureID();
    GLuint program = surface->GetProgramID();
    
    GameObject *owner = (*it)->GetOwner();
    Transform *transform = owner->GET<Transform>();
    
    // Get transforms in local and world space.
    Matrix33 modelTransform = transform->GetRotation() * Matrix33(transform->GetScale());
    Vector3 position = transform->GetPosition();
    TextureCoordinates *texCoord = surface->GetTextureData();
    Vector3 &size = transform->GetSize();
    Vector4 &color = surface->GetColor();
    Vector4 cameraTranslation;

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
    
    glPushMatrix();
    
    // Start using shader
    glUseProgram(program);
    int activeTexture = texture % GL_MAX_TEXTURE_UNITS;
    int vertexPosLocation = glGetAttribLocation(program, "vertexPos");
    printf("zjshs: %i,%i\n", vertexPosLocation, glGetError());
    int texCoordPosLocation = glGetAttribLocation(program, "texCoord");
    printf("zjshs: %i,%i\n", texCoordPosLocation, glGetError());
    
    // Vertex points
    PushRenderData(renderData, vertexPosLocation, topLeft);
    PushRenderData(renderData, texCoordPosLocation, Vector4(texCoord->GetXValue(0), texCoord->GetYValue(0), 0, 1));
    PushRenderData(renderData, vertexPosLocation, topRight);
    PushRenderData(renderData, texCoordPosLocation, Vector4(texCoord->GetXValue(1), texCoord->GetYValue(0), 0, 1));
    PushRenderData(renderData, vertexPosLocation, bottomRight);
    PushRenderData(renderData, texCoordPosLocation, Vector4(texCoord->GetXValue(1), texCoord->GetYValue(1), 0, 1));
    PushRenderData(renderData, vertexPosLocation, bottomLeft);
    PushRenderData(renderData, texCoordPosLocation, Vector4(texCoord->GetXValue(0), texCoord->GetYValue(1), 0, 1));
    
    // Enable textures and set uniforms.
    glBindVertexArray(mVertexArrayObjectID);
    glActiveTexture(GL_TEXTURE0 + activeTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "textureUnit"), activeTexture);
    glUniform3f(glGetUniformLocation(program, "objectPos"), position.x, position.y, position.z);
    glUniform4f(glGetUniformLocation(program, "primaryColor"), color.x, color.y, color.z, color.w);
    glUniform3f(glGetUniformLocation(program, "cameraDiff"), cameraTranslation.x, cameraTranslation.y, cameraTranslation.z);
    glUniform3f(glGetUniformLocation(program, "cameraSize"), cameraSize.x, cameraSize.y, cameraSize.z);
    glUniformMatrix3fv(glGetUniformLocation(program, "cameraTransform"), 1, GL_TRUE, cameraMatrix);
    
    // Set shader properties. Due to batching, done on a per surface / shader basis.
    // Shader uniforms are reset upon relinking.
    SetShaderProperties(surface, true);
    
    // Set VBO and buffer data.
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * renderData.size(), &renderData[0], GL_STATIC_DRAW);
    
    // For each attribute in a shader, we must enable Vertex Attribute Arrays.
    EnableVertexAttribArray(vertexPosLocation);
    EnableVertexAttribArray(texCoordPosLocation);
    glVertexAttribPointer(vertexPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4) * 2, 0);
    glVertexAttribPointer(texCoordPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4) * 2, (GLvoid*)(sizeof(Vector4)));
    glDrawArrays(GL_QUADS, 0, static_cast<int>(renderData.size()) / 2);
    DisableVertexAttribArray(vertexPosLocation);
    DisableVertexAttribArray(texCoordPosLocation);
    
    // Reset shader property values.
    SetShaderProperties(surface, false);
    
    glPopMatrix();

    // Reset to default texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    renderData.clear();
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
  glDisable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);

  glLoadIdentity();
  
  glGenVertexArrays(1, &mVertexArrayObjectID);
  glGenBuffers(1, &mVertexBufferID);
  glBindVertexArray(mVertexArrayObjectID);
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

void PCShaderScreen::EnableVertexAttribArray(int aVertexAttrib)
{
  // -1 is error state
  if(aVertexAttrib > -1)
  {
    glEnableVertexAttribArray(aVertexAttrib);
  }
}

void PCShaderScreen::DisableVertexAttribArray(int aVertexAttrib)
{
  // -1 is error state
  if(aVertexAttrib > -1)
  {
    glDisableVertexAttribArray(aVertexAttrib);
  }
}

void PCShaderScreen::PushRenderData(std::vector<Vector4> &aRenderData, int aAttribLocation, Vector4 const &aAttribute)
{
  // -1 is error state
  if(aAttribLocation > -1)
  {
    aRenderData.push_back(aAttribute);
  }
}
