#include "PCShaderScreen.h"
#include "PhysicsObject.h"
#include "PCShaderSurface.h"
#include "Constants.h"
#include "SystemProperties.h"
#include "SurfaceProperty.h"
#include "GraphicsManager.h"
#include "ShaderLoader.h"

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

PCShaderScreen::PCShaderScreen(GraphicsManager *aOwner, int aW, int aH, bool aFullScreen) : Screen(aOwner, aW, aH, aFullScreen), 
  mWindow(nullptr), mGLContext(), mDisplayMode(), mDefaultFrameBufferID(0), mVertexArrayObjectID(0), mVertexBufferID(0), 
  mTextureBufferID(0), mPositionBufferID(0), mColorBufferID(0), mIndexBufferID(0), mNormalBufferID(0), mMaxTextures(0)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  
#if defined(__APPLE__)
  glewExperimental = GL_TRUE;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetSwapInterval(1);
  // TODO, detect retina display, add SDL_WINDOW_ALLOW_HIGHDPI flag
#endif
  
  mWindow = SDL_CreateWindow(SystemProperties::GetGameTitle().ToCharArray(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aW, aH,
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
  
#if defined(__APPLE__)
  mFrameBuffer = new GLFramebuffer(SystemProperties::GetRenderWidth(), SystemProperties::GetRenderHeight());
  mFrameBuffer->Generate(GetOwner());
#endif
}

PCShaderScreen::~PCShaderScreen()
{
#if defined(__APPLE__)
  delete mFrameBuffer;
#endif
  
  glDeleteBuffers(1, &mVertexBufferID);
  glDeleteBuffers(1, &mTextureBufferID);
  glDeleteBuffers(1, &mPositionBufferID);
  glDeleteBuffers(1, &mColorBufferID);
  glDeleteBuffers(1, &mIndexBufferID);
  glDeleteBuffers(1, &mNormalBufferID);
  glDeleteVertexArrays(1, &mVertexArrayObjectID);
  SDL_GL_DeleteContext(mGLContext);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

/**
 * @brief Draw debug lines around objects, physics boxes only.
 * @param aObjects
 */
void PCShaderScreen::DebugDraw(std::vector<Surface*> const &aObjects)
{
  // Draw debug hitboxes for objects in environment, requires PhysicsObject
  // Camera position and size
  Transform *cameraTransform = GetOwner()->GetPrimaryCamera()->GetOwner()->GET<Transform>();
  Vector3 &cameraPosition = cameraTransform->GetPosition();
  Vector3 cameraSize = GetOwner()->GetPrimaryCamera()->GetSize() / 2.0f;
  Matrix33 viewMatrix = cameraTransform->GetFinalTransform();
  Vector3 cameraDiff = cameraPosition - cameraSize;
  
  float cameraMatrix[9];
  for(int i = 0; i < 9; ++i)
  {
    cameraMatrix[i] = viewMatrix.values[i/3][i%3];
  }
  
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end; ++it)
  {
    GameObject *obj = (*it)->GetOwner();

    if(obj->HAS<PhysicsObject>())
    {
      Transform *transform = obj->GET<Transform>();
      PhysicsObject *physicsObject = obj->GET<PhysicsObject>();
      Vector3 position = transform->GetHierarchicalPosition();
      Vector3 scale = transform->GetHierarchicalScale();
      Vector3 broadSize = physicsObject->GetBroadSize();
      Matrix33 rotation = transform->GetHierarchicalRotation();

      if((*it)->GetViewMode() == VIEW_ABSOLUTE)
      {
        position -= cameraDiff;
      }
      else if((*it)->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = (cameraSize.x * 2.0f) * position.x;
        position.y = (cameraSize.y * 2.0f) * position.y;
      }

      glLoadIdentity();
      glPushMatrix();
      
      // Stop using shaders
      glUseProgram(0);
      glBindTexture(GL_TEXTURE_2D, 0);

      // For each shape, draw the outline
      std::vector<Shape*> const& shapes = physicsObject->GetShapes();
      PhysicsObject::ConstShapeIT end = shapes.end();
      for(PhysicsObject::ConstShapeIT it = shapes.begin(); it != end; ++it)
      {
        if((*it)->shape == Shape::SPHERE)
        {
          Vector3 spherePos = position + (*it)->position.Multiply(rotation * scale);
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
        else if((*it)->shape == Shape::AABB)
        {
          Vector3 cubePos = position + (*it)->position.Multiply(rotation * scale);
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
          Vector3 triPos = position + (*it)->position.Multiply(rotation * scale);
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          for(int i = 0; i < 3; ++i)
          {
            Vector3 point = triPos + triangle->GetPoint(i).Multiply(rotation * scale);
            glVertex3f(point.x, point.y, point.z);
          }
          Vector3 point = triPos + triangle->GetPoint(0).Multiply(rotation * scale);
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
        else if((*it)->shape == Shape::OBB)
        {
          OrientedBoundingBox *obb = (OrientedBoundingBox*)(*it);
          Vector3 cubePos = position + (*it)->position.Multiply(rotation * scale);
          Vector3 extent0 = rotation * scale.Multiply(obb->GetAxis(0) * obb->GetSize(0));
          Vector3 extent1 = rotation * scale.Multiply(obb->GetAxis(1) * obb->GetSize(1));
          Vector3 pt1 = extent0 + extent1;
          Vector3 pt2 = extent0 - extent1;
          
          // Physics Box Line
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          glVertex3f(cubePos.x + pt1.x, cubePos.y + pt1.y, cubePos.z);
          glVertex3f(cubePos.x - pt2.x, cubePos.y - pt2.y, cubePos.z);
          glVertex3f(cubePos.x - pt1.x, cubePos.y - pt1.y, cubePos.z);
          glVertex3f(cubePos.x + pt2.x, cubePos.y + pt2.y, cubePos.z);
          glVertex3f(cubePos.x + pt1.x, cubePos.y + pt1.y, cubePos.z);
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
 * @brief Reset object texture on device reset.
 * @param aSurface Surface to reset.
 * @param aOldData Data pre reset.
 * @param aNewData Data post reset.
 */
void PCShaderScreen::ResetObjectTexture(Surface* aSurface, TextureData* aOldData, TextureData* aNewData)
{
  if(!aSurface)
    return;
  PCShaderSurface *surface = (PCShaderSurface*)aSurface;
  if(surface->GetTextureID() == aOldData->mTextureID)
    surface->SetTextureID(aNewData->mTextureID);
}

/**
 * @brief Reset object shader on device reset.
 * @param aSurface Surface to reset.
 * @param aOldData Data pre reset.
 * @param aNewData Data post reset.
 */
void PCShaderScreen::ResetObjectShader(Surface* aSurface, ShaderData* aOldData, ShaderData* aNewData)
{
  if(!aSurface)
    return;
  PCShaderSurface *surface = (PCShaderSurface*)aSurface;
  if(surface->GetProgramID() == aOldData->mProgramID)
    surface->SetProgramID(aNewData->mProgramID);
  if(surface->GetVertexShaderID() == aOldData->mVertexShaderID)
    surface->SetVertexShaderID(aNewData->mVertexShaderID);
  if(surface->GetFragmentShaderID() == aOldData->mFragmentShaderID)
    surface->SetFragmentShaderID(aNewData->mFragmentShaderID);
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
 * @param aUIObjects
 * @param aCamera
 */
void PCShaderScreen::Draw(std::vector<Surface*> const &aObjects, std::vector<Surface*> const &aUIObjects, Camera* aCamera)
{
// TODO Mac does not use multiple cameras with framebuffers
#ifndef __APPLE__
  aCamera->GetFramebuffer()->Bind();
  DrawObjects(aObjects, aCamera);
  DrawObjects(aUIObjects, aCamera);
  #ifdef _DEBUG_DRAW
    DebugDraw(aObjects);
  #endif
  aCamera->GetFramebuffer()->Unbind(mDefaultFrameBufferID);
  
  if(aCamera->GetPrimary())
    aCamera->GetFramebuffer()->Draw(GetWidth(), GetHeight(), mDisplayMode.w, mDisplayMode.h, IsFullScreen());
#else
  mFrameBuffer->Bind();
  if(!aCamera->GetPrimary())
    return;
  DrawObjects(aObjects, aCamera);
  DrawObjects(aUIObjects, aCamera);
  #ifdef _DEBUG_DRAW
    DebugDraw(aObjects);
  #endif
  mFrameBuffer->Unbind(mDefaultFrameBufferID);
  mFrameBuffer->Draw(GetWidth(), GetHeight(), mDisplayMode.w, mDisplayMode.h, IsFullScreen());
#endif
}

/**
 * @brief Swap buffers
 */
void PCShaderScreen::SwapBuffers()
{
  SDL_GL_SwapWindow(mWindow);
}

/**
 * @brief Set clear color.
 * @param aClearColor Color.
 */
void PCShaderScreen::SetClearColor(Vector4 const &aClearColor)
{
  glClearColor(aClearColor.x, aClearColor.y, aClearColor.z, aClearColor.w);
}

/**
 * @brief Change window size
 * @param aW Width
 * @param aH Height
 * @param aFullScreen
 */
void PCShaderScreen::ChangeSize(int aW, int aH, bool aFullScreen)
{
  int width = aW;
  int height = aH;
  float gameRatio = (float)aW / (float)aH;
  
  if(width > mDisplayMode.w)
  {
    width = mDisplayMode.w;
    height = width / gameRatio;
  }
  if(height > mDisplayMode.h)
  {
    height = mDisplayMode.h;
    width = height * gameRatio;
  }
  
  // Set full screen or not
  SetWidth(width);
  SetHeight(height);
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
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);

  if(aFullScreen)
  {
    float ratio = ((float)mDisplayMode.h / (float)height);
    int x = width * ratio;
    int y = height * ratio;
    glViewport((mDisplayMode.w - x)/2, (mDisplayMode.h - y)/2, width * ratio, mDisplayMode.h);
  }
  else
    glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, width, height, 0, 1, -1);
  //gluPerspective(45, (float)width / (float)height, 0.01f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glGenVertexArrays(1, &mVertexArrayObjectID);
  glGenBuffers(1, &mVertexBufferID);
  glGenBuffers(1, &mTextureBufferID);
  glGenBuffers(1, &mPositionBufferID);
  glGenBuffers(1, &mColorBufferID);
  glGenBuffers(1, &mIndexBufferID);
  glGenBuffers(1, &mNormalBufferID);
  
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mDefaultFrameBufferID);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mMaxTextures);
  
  GetOwner()->ResetDevice();
}

/**
 * @brief Draw objects from camera perspective
 * @param aObjects
 * @param aCamera
 */
void PCShaderScreen::DrawObjects(std::vector<Surface*> const &aObjects, Camera *aCamera)
{
  // Camera position and size
  Transform *cameraTransform = aCamera->GetOwner()->GET<Transform>();
  Vector3 cameraPosition = cameraTransform->GetPosition() + aCamera->GetOffset();
  Vector3 cameraSize = aCamera->GetSize() / 2.0f;
  Matrix33 viewMatrix = cameraTransform->GetFinalTransform();
  Matrix33 identityMatrix = Matrix33();
  
  // Must scale, rotate, then translate camera offset
  Vector3 cameraDiff = (viewMatrix * cameraPosition) - cameraSize;
  
  std::vector<Vector3> vertexData, positionData, normalData;
  std::vector<Vector2> textureData;
  std::vector<Vector4> colorData;
  std::vector<GLuint> indices;
  
  int numVertices = aObjects.size() * 6;
  vertexData.reserve(numVertices);
  textureData.reserve(numVertices);
  colorData.reserve(numVertices);
  positionData.reserve(numVertices);
  indices.reserve(numVertices);
  normalData.reserve(numVertices);
  
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
    Vector3 cameraTranslation;
    
    if(surface->GetNoRender())
    {
      ++it;
      continue;
    }
    
    // Start using shader
    glUseProgram(program);
    int activeTexture = 0;
    int vertexPosLocation = glGetAttribLocation(program, "vertexPos");
    int texCoordPosLocation = glGetAttribLocation(program, "texCoord");
    int objectPosLocation = glGetAttribLocation(program, "objectPos");
    int colorPosLocation = glGetAttribLocation(program, "primaryColor");
    int normalLocation = glGetAttribLocation(program, "normal");
    
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
    else // Menu objects do not rotate with camera.
    {
      for(int i = 0; i < 9; ++i)
      {
        cameraMatrix[i] = identityMatrix.values[i/3][i%3];
      }
    }
    
    int iteration = 0;
    while(it != end && (*it)->GetProgramID() == program &&
          (*it)->GetTextureID() == texture && (*it)->GetViewMode() == viewSpace)
    {
      GameObject *owner = (*it)->GetOwner();
      PCShaderSurface *surface = owner->GET<PCShaderSurface>();
      Transform *transform = owner->GET<Transform>();
      
      if(surface->GetNoRender())
      {
        ++it;
        continue;
      }
    
      // Get transforms in local and world space.
      Matrix33 modelTransform = transform->GetHierarchicalRotation() * Matrix33(transform->GetHierarchicalScale());
      Vector3 position = transform->GetHierarchicalPosition();
      TextureCoordinates *texCoord = surface->GetTextureData();
      Vector3 &size = transform->GetSize();
      Vector4 &color = surface->GetColor();
      
      // If transform is a percentage of screen, convert.
      if((*it)->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = (cameraSize.x * 2.0f) * position.x;
        position.y = (cameraSize.y * 2.0f) * position.y;
      }

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
      
      // Calculate normal
      Vector3 normal = topRight.Cross(topLeft).normalize();
      
      // Vertex points
      PushRenderDataV3(vertexData, vertexPosLocation, topLeft);
      PushRenderDataV3(vertexData, vertexPosLocation, bottomLeft);
      PushRenderDataV3(vertexData, vertexPosLocation, bottomRight);
      PushRenderDataV3(vertexData, vertexPosLocation, bottomRight);
      PushRenderDataV3(vertexData, vertexPosLocation, topRight);
      PushRenderDataV3(vertexData, vertexPosLocation, topLeft);
      
      // Texture coordinates
      PushRenderDataV2(textureData, texCoordPosLocation, Vector2(texCoord->GetXValue(0), texCoord->GetYValue(0)));
      PushRenderDataV2(textureData, texCoordPosLocation, Vector2(texCoord->GetXValue(0), texCoord->GetYValue(1)));
      PushRenderDataV2(textureData, texCoordPosLocation, Vector2(texCoord->GetXValue(1), texCoord->GetYValue(1)));
      PushRenderDataV2(textureData, texCoordPosLocation, Vector2(texCoord->GetXValue(1), texCoord->GetYValue(1)));
      PushRenderDataV2(textureData, texCoordPosLocation, Vector2(texCoord->GetXValue(1), texCoord->GetYValue(0)));
      PushRenderDataV2(textureData, texCoordPosLocation, Vector2(texCoord->GetXValue(0), texCoord->GetYValue(0)));
      
      // Position data
      PushRenderDataV3(positionData, objectPosLocation, position);
      PushRenderDataV3(positionData, objectPosLocation, position);
      PushRenderDataV3(positionData, objectPosLocation, position);
      PushRenderDataV3(positionData, objectPosLocation, position);
      PushRenderDataV3(positionData, objectPosLocation, position);
      PushRenderDataV3(positionData, objectPosLocation, position);
      
      // Color data
      PushRenderDataV4(colorData, colorPosLocation, color);
      PushRenderDataV4(colorData, colorPosLocation, color);
      PushRenderDataV4(colorData, colorPosLocation, color);
      PushRenderDataV4(colorData, colorPosLocation, color);
      PushRenderDataV4(colorData, colorPosLocation, color);
      PushRenderDataV4(colorData, colorPosLocation, color);
      
      // Normal data
      PushRenderDataV3(normalData, normalLocation, normal);
      PushRenderDataV3(normalData, normalLocation, normal);
      PushRenderDataV3(normalData, normalLocation, normal);
      PushRenderDataV3(normalData, normalLocation, normal);
      PushRenderDataV3(normalData, normalLocation, normal);
      PushRenderDataV3(normalData, normalLocation, normal);
      
      for(int i = 0; i < 6; ++i)
      {
        indices.push_back(i + (iteration * 6));
      }
      
      // If this object has properties, separate into its own draw call.
      if((*it)->GetProperties().size() != 0)
      {
        ++it;
        break;
      }
      else
      {
        ++it;
        
        if(it != end && (*it)->GetProperties().size() != 0)
        {
          break;
        }
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
    BindAttributeV3(GL_ARRAY_BUFFER, mVertexBufferID, vertexPosLocation, vertexData);
    BindAttributeV2(GL_ARRAY_BUFFER, mTextureBufferID, texCoordPosLocation, textureData);
    BindAttributeV3(GL_ARRAY_BUFFER, mPositionBufferID, objectPosLocation, positionData);
    BindAttributeV4(GL_ARRAY_BUFFER, mColorBufferID, colorPosLocation, colorData);
    BindAttributeV3(GL_ARRAY_BUFFER, mNormalBufferID, normalLocation, normalData);
    
    // Set index data
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    vertexData.clear();
    textureData.clear();
    colorData.clear();
    positionData.clear();
    indices.clear();
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
 * @brief Set shader properties based on surface. HINT: If you're not seeing your intended results, remember
 *        to declare the field as uniform in your shader.
 * @param aSurface Surface to set properties for.
 * @param aActive Decide whether or not to set values or reset.
 */
void PCShaderScreen::SetShaderProperties(PCShaderSurface *aSurface, bool aActive)
{
  // Set properties for shader. Separated by program id.
  GLuint program = aSurface->GetProgramID();
  PropertyContainer const &properties = aSurface->GetProperties();
  PropertyContainerConstIt propertyEnd = properties.end();
  for(PropertyContainerConstIt propertyIt = properties.begin(); propertyIt != propertyEnd; ++propertyIt)
  {
    SurfaceProperty *property = *propertyIt;
    HashString value = property->GetTargetValue();
    
    if(!aActive)
      value = property->GetDefaultValue();
      
    #ifdef _DEBUG_DRAW
      DebugLogPrint("Setting uniform %s to %s\n", property->GetName().ToCharArray(), value.ToCharArray());
    #endif
    
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
 * @brief Disable attribute array
 * @param aVertexAttrib Index of attribute
 */
void PCShaderScreen::DisableVertexAttribArray(int aVertexAttrib)
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
void PCShaderScreen::PushRenderDataV2(std::vector<Vector2> &aData, int aAttribLocation, Vector2 const &aAttribute)
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
void PCShaderScreen::PushRenderDataV3(std::vector<Vector3> &aData, int aAttribLocation, Vector3 const &aAttribute)
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
void PCShaderScreen::PushRenderDataV4(std::vector<Vector4> &aData, int aAttribLocation, Vector4 const &aAttribute)
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
void PCShaderScreen::BindAttributeV2(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector2> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    glBindBuffer(aTarget, aBufferID);
    glBufferData(aTarget, sizeof(Vector2) * aData.size(), &aData[0], GL_DYNAMIC_DRAW);
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
void PCShaderScreen::BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    glBindBuffer(aTarget, aBufferID);
    glBufferData(aTarget, sizeof(Vector3) * aData.size(), &aData[0], GL_DYNAMIC_DRAW);
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
void PCShaderScreen::BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    glBindBuffer(aTarget, aBufferID);
    glBufferData(aTarget, sizeof(Vector4) * aData.size(), &aData[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(aAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    glBindBuffer(aTarget, 0);
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
