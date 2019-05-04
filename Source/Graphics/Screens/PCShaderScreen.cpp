#include "PCShaderScreen.h"
#include "PhysicsObject.h"
#include "PCShaderSurface.h"
#include "Constants.h"
#include "SystemProperties.h"
#include "RenderableProperty.h"
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
  
  glewExperimental = GL_TRUE;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetSwapInterval(1);
  // TODO, detect retina display, add SDL_WINDOW_ALLOW_HIGHDPI flag
  
  mWindow = SDL_CreateWindow(SystemProperties::GetGameTitle().ToCharArray(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aW, aH,
                             SDL_WINDOW_OPENGL);
  mGLContext = SDL_GL_CreateContext(mWindow);
  SDL_GL_MakeCurrent(mWindow, mGLContext);
  SDL_GetDesktopDisplayMode(0, &mDisplayMode);
  
  GLenum glError = glewInit();
  GL_ERROR_CHECK();
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
void PCShaderScreen::DebugDraw(std::vector<Renderable*> const &aObjects)
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
  
  std::vector<Renderable*>::const_iterator end = aObjects.end();
  for(std::vector<Renderable*>::const_iterator it = aObjects.begin(); it != end; ++it)
  {
    GameObject *obj = (*it)->GetOwner();

    if(obj->HAS<PhysicsObject>())
    {
      Transform *transform = obj->GET<Transform>();
      PhysicsObject *physicsObject = obj->GET<PhysicsObject>();
      Vector3 position = transform->GetHierarchicalPosition();
      Matrix33 scale = Matrix33(transform->GetHierarchicalScale());
      Vector3 broadSize = physicsObject->GetBroadSize();
      Matrix33 rotation = transform->GetHierarchicalRotation();
      Matrix33 modelMatrix = scale * rotation;

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
      GL_ERROR_CHECK();
      glPushMatrix();
      GL_ERROR_CHECK();
      
      // Stop using shaders
      glUseProgram(0);
      GL_ERROR_CHECK();
      glBindTexture(GL_TEXTURE_2D, 0);
      GL_ERROR_CHECK();

      // For each shape, draw the outline
      std::vector<Shape*> const& shapes = physicsObject->GetShapes();
      PhysicsObject::ConstShapeIT end = shapes.end();
      for(PhysicsObject::ConstShapeIT it = shapes.begin(); it != end; ++it)
      {
        if((*it)->passable)
          glColor3f(1.0f, 0.0f, 1.0f);
        else
          glColor3f(1.0f, 0.0f, 0.0f);
        GL_ERROR_CHECK();
          
        if((*it)->shape == Shape::SPHERE)
        {
          Vector3 spherePos = position + (modelMatrix * (*it)->position);
          glBegin(GL_LINE_STRIP);
          GL_ERROR_CHECK();
          
          // Rotate in a circle
          for(int i = 0; i < 360; ++i)
          {
            float radians = i * DEGREE_TO_RADS;
            float x = (*it)->GetSize(0) * cos(radians) * scale.values[0][0];
            float y = (*it)->GetSize(1) * sin(radians) * scale.values[1][1];
            glVertex3f(spherePos.x + x, spherePos.y + y, spherePos.z);
            GL_ERROR_CHECK();
          }
          glEnd();
          GL_ERROR_CHECK();
        }
        else if((*it)->shape == Shape::AABB)
        {
          Vector3 cubePos = position + (modelMatrix * (*it)->position);
          float xSize = (*it)->GetSize(0) * scale.values[0][0];
          float ySize = (*it)->GetSize(1) * scale.values[1][1];
          
          // Physics Box Line
          glBegin(GL_LINE_STRIP);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x - xSize, cubePos.y - ySize, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x + xSize, cubePos.y - ySize, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x + xSize, cubePos.y + ySize, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x - xSize, cubePos.y + ySize, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x - xSize, cubePos.y - ySize, cubePos.z);
          GL_ERROR_CHECK();
          glEnd();
          GL_ERROR_CHECK();
        }
        else if((*it)->shape == Shape::TRIANGLE)
        {
          Triangle* triangle = (Triangle*)(*it);
          Vector3 triPos = position + (modelMatrix * (*it)->position);
          glBegin(GL_LINE_STRIP);
          GL_ERROR_CHECK();
          for(int i = 0; i < 3; ++i)
          {
            Vector3 point = triPos + (modelMatrix* triangle->GetPoint(i));
            glVertex3f(point.x, point.y, point.z);
            GL_ERROR_CHECK();
          }
          Vector3 point = triPos + (modelMatrix * triangle->GetPoint(0));
          glVertex3f(point.x, point.y, point.z);
          GL_ERROR_CHECK();
          glEnd();
          GL_ERROR_CHECK();
        }
        else if((*it)->shape == Shape::LINE)
        {
          Line *line = (Line*)(*it);
          Vector3 linePos = position + scale * (*it)->position;
          Vector3 lineEnd = linePos + scale * line->direction * line->length;
          glBegin(GL_LINE_STRIP);
          GL_ERROR_CHECK();
          glVertex3f(linePos.x, linePos.y, linePos.z);
          GL_ERROR_CHECK();
          glVertex3f(lineEnd.x, lineEnd.y, lineEnd.z);
          GL_ERROR_CHECK();
          glEnd();
          GL_ERROR_CHECK();
        }
        else if((*it)->shape == Shape::OBB)
        {
          OrientedBoundingBox *obb = (OrientedBoundingBox*)(*it);
          Vector3 cubePos = position + (modelMatrix * (*it)->position);
          Vector3 extent0 = rotation * scale * obb->GetAxis(0) * obb->GetSize(0);
          Vector3 extent1 = rotation * scale * obb->GetAxis(1) * obb->GetSize(1);
          Vector3 pt1 = extent0 + extent1;
          Vector3 pt2 = extent0 - extent1;
          
          // Physics Box Line
          glBegin(GL_LINE_STRIP);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x + pt1.x, cubePos.y + pt1.y, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x - pt2.x, cubePos.y - pt2.y, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x - pt1.x, cubePos.y - pt1.y, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x + pt2.x, cubePos.y + pt2.y, cubePos.z);
          GL_ERROR_CHECK();
          glVertex3f(cubePos.x + pt1.x, cubePos.y + pt1.y, cubePos.z);
          GL_ERROR_CHECK();
          glEnd();
          GL_ERROR_CHECK();
        }
      }

      // Broad Size Line
      glBegin(GL_LINE_STRIP);
      GL_ERROR_CHECK();
      glColor3f(1.0f, 1.0f, 0.0f);
      GL_ERROR_CHECK();
      glVertex3f(position.x - broadSize.x, position.y - broadSize.y, position.z);
      GL_ERROR_CHECK();
      glVertex3f(position.x + broadSize.x, position.y - broadSize.y, position.z);
      GL_ERROR_CHECK();
      glVertex3f(position.x + broadSize.x, position.y + broadSize.y, position.z);
      GL_ERROR_CHECK();
      glVertex3f(position.x - broadSize.x, position.y + broadSize.y, position.z);
      GL_ERROR_CHECK();
      glVertex3f(position.x - broadSize.x, position.y - broadSize.y, position.z);
      GL_ERROR_CHECK();
      glEnd();
      GL_ERROR_CHECK();
      glPopMatrix();
      GL_ERROR_CHECK();
    }
  }
}

/**
 * @brief Set shader property globally for program. HINT: If you're not seeing your intended results, remember
 *        to declare the field as uniform in your shader.
 * @param aShaderData Shader information, specifically program id.
 * @param aProperty Property to set uniform to.
 */
void PCShaderScreen::SetGlobalShaderProperty(ShaderData *aShaderData, RenderableProperty const &aProperty)
{
  int program = aShaderData->mProgramID;
  glUseProgram(program);
  ShaderLoader::SetShaderUniform(program, aProperty.GetName(), aProperty.GetType(), aProperty.GetTargetValue(), aProperty.GetId());
}

/**
 * @brief Reset object texture on device reset.
 * @param aRenderable Renderable to reset.
 * @param aOldData Data pre reset.
 * @param aNewData Data post reset.
 */
void PCShaderScreen::ResetObjectTexture(Renderable* aRenderable, TextureData* aOldData, TextureData* aNewData)
{
  if(!aRenderable)
    return;
  PCShaderSurface *surface = (PCShaderSurface*)aRenderable;
  if(surface->GetTextureID() == aOldData->mTextureID)
    surface->SetTextureID(aNewData->mTextureID);
}

/**
 * @brief Reset object shader on device reset.
 * @param aRenderable Renderable to reset.
 * @param aOldData Data pre reset.
 * @param aNewData Data post reset.
 */
void PCShaderScreen::ResetObjectShader(Renderable* aRenderable, ShaderData* aOldData, ShaderData* aNewData)
{
  if(!aRenderable)
    return;
  PCShaderSurface *surface = (PCShaderSurface*)aRenderable;
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
  GL_ERROR_CHECK();
}

/**
 * @brief Draw objects
 * @param aObjects
 * @param aCamera
 */
void PCShaderScreen::Draw(std::map<int, std::vector<Renderable*>> const &aObjects, Camera* aCamera)
{
  // Isolate objects
  std::map<int, std::vector<Renderable*>> isolatedRenderObjects;
  for(std::map<int, std::vector<Renderable*>>::const_iterator it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    for(std::vector<Renderable*>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    {
      Renderable *renderable = *it2;
      for(std::vector<int>::const_iterator it3 = renderable->GetIsolatedRenderLayers().begin(); it3 != renderable->GetIsolatedRenderLayers().end(); ++it3)
      {
        if(isolatedRenderObjects.find(*it3) == isolatedRenderObjects.end())
        {
          isolatedRenderObjects[*it3].reserve(it->second.size());
        }
        isolatedRenderObjects[*it3].push_back(renderable);
      }
    }
  }
  
  // Draw objects that request isolated layering.
  std::vector<int> inputTextures;
  for(std::map<int, std::vector<Renderable*>>::iterator it = isolatedRenderObjects.begin(); it != isolatedRenderObjects.end(); ++it)
  {
    aCamera->GetFramebuffer(it->first)->SetClearColor(Vector4(0,0,0,0));
    aCamera->GetFramebuffer(it->first)->Bind();
    DrawObjects(it->second, aCamera, std::vector<int>());
    aCamera->GetFramebuffer(it->first)->Unbind(mDefaultFrameBufferID);
    inputTextures.push_back(aCamera->GetFramebuffer(it->first)->GetTextureID());
  }
  
  // Draw everything with ordering intact
  aCamera->GetFramebuffer(0)->Bind();
  for(std::map<int, std::vector<Renderable*>>::const_iterator it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    DrawObjects(it->second, aCamera, inputTextures);
#ifdef _DEBUG_DRAW
    //DebugDraw(it->second);
#endif
  }
  aCamera->GetFramebuffer(0)->Unbind(mDefaultFrameBufferID);
  
  // If primary, run through pipeline
  if(aCamera->GetPrimary())
  {
    Pipeline *pipeline = GetOwner()->GetPipeline(aCamera->GetPipelineName().ToHash());
    Framebuffer *result = pipeline->Run(aCamera->GetFramebuffers());
    result->Draw(GetWidth(), GetHeight(), mDisplayMode.w, mDisplayMode.h, IsFullScreen());
  }
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
  GL_ERROR_CHECK();
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

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  GL_ERROR_CHECK();
  glClearDepth(0.0f);
  GL_ERROR_CHECK();
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GL_ERROR_CHECK();
  glEnable(GL_TEXTURE_2D);
  GL_ERROR_CHECK();
  glEnable(GL_BLEND);
  GL_ERROR_CHECK();
  glEnable(GL_CULL_FACE);
  GL_ERROR_CHECK();
  glDisable(GL_DEPTH_TEST);
  GL_ERROR_CHECK();
  glDepthMask(GL_TRUE);
  GL_ERROR_CHECK();
  glDepthFunc(GL_GREATER);
  GL_ERROR_CHECK();
  if(aFullScreen)
  {
    float ratio = ((float)mDisplayMode.h / (float)height);
    int x = width * ratio;
    int y = height * ratio;
    if(x <= mDisplayMode.w)
      glViewport((mDisplayMode.w - x)/2, (mDisplayMode.h - y)/2, x, mDisplayMode.h);
    else
    {
      ratio = ((float)mDisplayMode.w / (float)width);
      x = width * ratio;
      y = height * ratio;
      glViewport((mDisplayMode.w - x)/2, (mDisplayMode.h - y)/2, mDisplayMode.w, y);
    }
  }
  else
    glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  GL_ERROR_CHECK();
  glLoadIdentity();
  GL_ERROR_CHECK();

  glOrtho(0, width, height, 0, 1, -1);
  //gluPerspective(45, (float)width / (float)height, 1, -1);
  GL_ERROR_CHECK();

  glMatrixMode(GL_MODELVIEW);
  GL_ERROR_CHECK();
  glLoadIdentity();
  GL_ERROR_CHECK();
  
  glGenVertexArrays(1, &mVertexArrayObjectID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mVertexBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mTextureBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mPositionBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mColorBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mIndexBufferID);
  GL_ERROR_CHECK();
  glGenBuffers(1, &mNormalBufferID);
  GL_ERROR_CHECK();
  
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mDefaultFrameBufferID);
  GL_ERROR_CHECK();
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mMaxTextures);
  GL_ERROR_CHECK();
  
  GetOwner()->ResetDevice();
}

/**
 * @brief Draw objects from camera perspective
 * @param aObjects
 * @param aCamera
 * @param aInputTextures
 * @return Objects that need to be rendered to own layers.
 */
void PCShaderScreen::DrawObjects(std::vector<Renderable*> const &aObjects, Camera *aCamera, std::vector<int> const &aInputTextures)
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
  int activeTexture = 0;
  std::vector<Renderable*>::const_iterator end = aObjects.end();
  for(std::vector<Renderable*>::const_iterator it = aObjects.begin(); it != end;)
  {
    // Get the texture id of the surface
    Renderable *surface = *it;
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
    GL_ERROR_CHECK();
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
      Renderable *surface = *it;
      Transform *transform = owner->GET<Transform>();
      
      if(surface->GetNoRender())
      {
        ++it;
        continue;
      }
    
      // Get transforms in local and world space.
      Matrix33 modelTransform = transform->GetHierarchicalRotation() * Matrix33(transform->GetHierarchicalScale());
      Vector3 position = transform->GetHierarchicalPosition();
      Vector3 &size = transform->GetSize();
      
      // If transform is a percentage of screen, convert.
      if((*it)->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = (cameraSize.x * 2.0f) * position.x;
        position.y = (cameraSize.y * 2.0f) * position.y;
      }

      VertexContainer vertices = surface->GetVertexData(position);
      for(VertexIT it = vertices.begin(); it != vertices.end(); ++it)
      {
        VertexData &vertex = *it;
        PushRenderDataV3(positionData, objectPosLocation, vertex.mPosition);
        PushRenderDataV3(vertexData, vertexPosLocation, vertex.mVertex);
        PushRenderDataV2(textureData, texCoordPosLocation, vertex.mTextureCoord);
        PushRenderDataV4(colorData, colorPosLocation, vertex.mColor);
        PushRenderDataV3(normalData, normalLocation, vertex.mNormal);
      }
      
      for(int i = 0; i < 6; ++i)
      {
        indices.push_back(i + (iteration * 6));
      }
      
      // If this object has different properties, separate into its own draw call.
      ++it;
      if(it != end && !(*it)->RenderablePropertiesEquals(surface))
      {
        break;
      }
      
      ++iteration;
    }
    
    // Enable textures and set uniforms.
    glBindVertexArray(mVertexArrayObjectID);
    GL_ERROR_CHECK();
    glActiveTexture(GL_TEXTURE0 + activeTexture);
    GL_ERROR_CHECK();
    glBindTexture(GL_TEXTURE_2D, texture);
    GL_ERROR_CHECK();
    glUniform1i(glGetUniformLocation(program, "textureUnit"), activeTexture);
    GL_ERROR_CHECK();
    glUniform3f(glGetUniformLocation(program, "cameraDiff"), cameraTranslation.x, cameraTranslation.y, cameraTranslation.z);
    GL_ERROR_CHECK();
    glUniform3f(glGetUniformLocation(program, "cameraSize"), cameraSize.x, cameraSize.y, cameraSize.z);
    GL_ERROR_CHECK();
    glUniformMatrix3fv(glGetUniformLocation(program, "cameraTransform"), 1, GL_TRUE, cameraMatrix);
    GL_ERROR_CHECK();
    
    // Set optional uniforms.
    SetOptionalUniforms(surface, aInputTextures);
    
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
    GL_ERROR_CHECK();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);
    GL_ERROR_CHECK();
    
    // Draw and disable
    glDrawElements(GL_TRIANGLES, static_cast<unsigned>(vertexData.size()), GL_UNSIGNED_INT, 0);
    GL_ERROR_CHECK();
    DisableVertexAttribArray(vertexPosLocation);
    DisableVertexAttribArray(texCoordPosLocation);
    DisableVertexAttribArray(objectPosLocation);
    DisableVertexAttribArray(colorPosLocation);
    
    // Reset shader property values.
    SetShaderProperties(surface, false);
    
    vertexData.clear();
    textureData.clear();
    colorData.clear();
    positionData.clear();
    indices.clear();
  }
}

/**
 * @brief Set optional uniforms for surface.
 * @param aRenderable Renderable.
 * @param aInputTextures Optional input textures.
 */
void PCShaderScreen::SetOptionalUniforms(Renderable* aRenderable, std::vector<int> const &aInputTextures)
{
  GLint location = -1;
  location = glGetUniformLocation(aRenderable->GetProgramID(), "textureWidth");
  GL_ERROR_CHECK();
  if(location != -1)
  {
    glUniform1i(location, static_cast<int>(aRenderable->GetTextureSize().x));
    GL_ERROR_CHECK();
  }
  location = glGetUniformLocation(aRenderable->GetProgramID(), "textureHeight");
  GL_ERROR_CHECK();
  if(location != -1)
  {
    glUniform1i(location, static_cast<int>(aRenderable->GetTextureSize().y));
    GL_ERROR_CHECK();
  }
  
  int i = 1;
  for(std::vector<int>::const_iterator it = aInputTextures.begin(); it != aInputTextures.end(); ++it, i++)
  {
    HashString name = HashString("inputTextureUnit") + Common::IntToString(i - 1);
    if(glGetUniformLocation(aRenderable->GetProgramID(), name.ToCharArray()) != -1)
    {
      int value = *it;
      GL_ERROR_CHECK();
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, value);
      glUniform1i(glGetUniformLocation(aRenderable->GetProgramID(), name.ToCharArray()), i);
      GL_ERROR_CHECK();
    }
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
 * @param aRenderable Renderable to set properties for.
 * @param aActive Decide whether or not to set values or reset.
 */
void PCShaderScreen::SetShaderProperties(Renderable *aRenderable, bool aActive)
{
  // Set properties for shader. Separated by program id.
  GLuint program = aRenderable->GetProgramID();
  PropertyContainer const &properties = aRenderable->GetProperties();
  PropertyContainerConstIt propertyEnd = properties.end();
  for(PropertyContainerConstIt propertyIt = properties.begin(); propertyIt != propertyEnd; ++propertyIt)
  {
    RenderableProperty *property = propertyIt->second;
    HashString value = property->GetTargetValue();
    
    if(!aActive)
      value = property->GetDefaultValue();
      
    #ifdef _DEBUG_DRAW
      //DebugLogPrint("Setting uniform %s to %s\n", property->GetName().ToCharArray(), value.ToCharArray());
    #endif
    
    ShaderLoader::SetShaderUniform(program, property->GetName(), property->GetType(), value, property->GetId());
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
    GL_ERROR_CHECK();
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
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, aBufferID);
    GL_ERROR_CHECK();
    glBufferData(aTarget, sizeof(Vector2) * aData.size(), &aData[0], GL_DYNAMIC_DRAW);
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
void PCShaderScreen::BindAttributeV3(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector3> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, aBufferID);
    GL_ERROR_CHECK();
    glBufferData(aTarget, sizeof(Vector3) * aData.size(), &aData[0], GL_DYNAMIC_DRAW);
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
void PCShaderScreen::BindAttributeV4(GLenum aTarget, int const aBufferID, int const aAttributeLocation, std::vector<Vector4> &aData)
{
  if(aAttributeLocation != -1)
  {
    glEnableVertexAttribArray(aAttributeLocation);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, aBufferID);
    GL_ERROR_CHECK();
    glBufferData(aTarget, sizeof(Vector4) * aData.size(), &aData[0], GL_DYNAMIC_DRAW);
    GL_ERROR_CHECK();
    glVertexAttribPointer(aAttributeLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
    GL_ERROR_CHECK();
    glBindBuffer(aTarget, 0);
    GL_ERROR_CHECK();
  }
}
