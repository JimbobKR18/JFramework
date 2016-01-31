#include "PCShaderScreen.h"
#include "PhysicsObject.h"
#include "PCShaderSurface.h"
#include "Constants.h"

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

PCShaderScreen::PCShaderScreen(int aW, int aH) : Screen(aW, aH)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  mWindow = SDL_CreateWindow(Constants::GetString("GameTitle").ToCharArray(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, aW, aH, 
                             SDL_WINDOW_OPENGL);
  mGLContext = SDL_GL_CreateContext(mWindow);
  GLenum glError = glewInit();
  if(glError != GLEW_OK)
  {
    DebugLogPrint("Error: %s\n", glewGetErrorString(glError));
    assert(!"GLEW failed to init.");
  }
  
  ChangeSize(aW, aH, Constants::GetBoolean("FullScreen"));
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
  Vector3 cameraPosition = GetView().GetPosition();
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
      Vector3 broadSize = physicsObject->GetBroadSize();

      // Get positions relative to the camera
      float xPosition = position.x;
      float yPosition = position.y;
      float zPosition = position.z;

      if((*it)->GetViewMode() == VIEW_ABSOLUTE)
      {
        xPosition -= cameraDiff.x;
        yPosition -= cameraDiff.y;
        zPosition -= cameraDiff.z;
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
          Vector3 spherePos = Vector3(xPosition, yPosition, zPosition) +
                              (*it)->position;
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          
          // Rotate in a circle
          for(int i = 0; i < 360; ++i)
          {
            float radians = i * DEGREE_TO_RADS;
            glVertex3f(spherePos.x + ((*it)->GetSize(0) * cos(radians)),
                spherePos.y + ((*it)->GetSize(1) * sin(radians)), spherePos.z);
          }
          glEnd();
        }
        else if((*it)->shape == Shape::CUBE)
        {
          Vector3 cubePos = Vector3(xPosition, yPosition, zPosition) +
                              (*it)->position;
          float xSize = (*it)->GetSize(0);
          float ySize = (*it)->GetSize(1);
          
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
          Vector3 triPos = Vector3(xPosition, yPosition, zPosition) +
                              (*it)->position;
          glBegin(GL_LINE_STRIP);
          glColor3f(1.0f, 0.0f, 0.0f);
          for(int i = 0; i < 3; ++i)
          {
            Vector3 point = triPos + triangle->GetPoint(i);
            glVertex3f(point.x, point.y, point.z);
          }
          Vector3 point = triPos + triangle->GetPoint(0);
          glVertex3f(point.x, point.y, point.z);
          glEnd();
        }
        else if((*it)->shape == Shape::LINE)
        {
          Line *line = (Line*)(*it);
          Vector3 linePos = Vector3(xPosition, yPosition, zPosition) +
                            (*it)->position;
          Vector3 lineEnd = linePos + (line->direction * line->length);
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
      glVertex3f(xPosition - broadSize.x, yPosition - broadSize.y, zPosition);
      glVertex3f(xPosition + broadSize.x, yPosition - broadSize.y, zPosition);
      glVertex3f(xPosition + broadSize.x, yPosition + broadSize.y, zPosition);
      glVertex3f(xPosition - broadSize.x, yPosition + broadSize.y, zPosition);
      glVertex3f(xPosition - broadSize.x, yPosition - broadSize.y, zPosition);
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
  // TODO this is almost complete, have to pass camera and object params to shader.
  // Camera position and size
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraScale = GetView().GetScale();
  Vector3 cameraSize = GetView().GetHalfSize();
  Matrix33 cameraRotation = GetView().GetRotation();
  
  // Must scale, rotate, then translate camera offset
  Vector3 cameraDiff = (cameraRotation * cameraPosition.Multiply(cameraScale)) - cameraSize;
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end;)
  {
    // Get the texture id of the surface
    GLuint texture = (*it)->GetOwner()->GET<PCShaderSurface>()->GetTextureID();
    GLuint program = (*it)->GetOwner()->GET<PCShaderSurface>()->GetProgramID();
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glPushMatrix();
    
    std::vector<Vector4> renderData;
    renderData.reserve(1024);
    
    // While other texture share the same texture id, draw them
    while(it != end &&
          (*it)->GetOwner()->GET<PCShaderSurface>()->GetTextureID() == texture)
    {
      GameObject *owner = (*it)->GetOwner();
      PCShaderSurface *surface = (PCShaderSurface*)(*it);
      Transform *transform = owner->GET<Transform>();
      
      // Gotta progress this somehow
      ++it;
      
      // Get position, rotation, scale, and size
      Vector3 position = transform->GetPosition();
      Matrix33 rotation = transform->GetRotation() * cameraRotation;
      Vector3 size = transform->GetSize();
      Vector3 scale = transform->GetScale();
      
      TextureCoordinates *texCoord = surface->GetTextureData();
      Vector4 color = surface->GetColor();
      
      // Camera scale
      size *= cameraScale;
      position *= cameraScale;
      
      // Camera rotation
      position = cameraRotation * position;

      // Camera translation
      if(surface->GetViewMode() == VIEW_ABSOLUTE)
      {
        position -= cameraDiff;
      }

      // Move object based on its alignment
      AlignmentHelper(transform, size, position);

      // Local scaling
      size *= scale;
      
      // Get the basic coordinates for the quad
      Vector3 topLeft = Vector3(-size.x, -size.y, 0);
      Vector3 topRight = Vector3(size.x, -size.y, 0);
      Vector3 bottomRight = Vector3(size.x, size.y, 0);
      Vector3 bottomLeft = Vector3(-size.x, size.y, 0);
      
      // Local rotate
      topLeft = rotation * topLeft;
      topRight = rotation * topRight;
      bottomLeft = rotation * bottomLeft;
      bottomRight = rotation * bottomRight;

      // Local translate
      topLeft += position;
      topRight += position;
      bottomRight += position;
      bottomLeft += position;
      
      // Determine whether or not to draw (If any point is on screen or box collides)
      bool draw = PointIsOnScreen(topLeft) || PointIsOnScreen(topRight) || 
        PointIsOnScreen(bottomLeft) || PointIsOnScreen(bottomRight) ||
        BoxIsOnScreen(topLeft, bottomRight);
      if(!draw)
      {
        continue;
      }
      
      // Vertex points
      renderData.push_back(topLeft);
      renderData.push_back(topRight);
      renderData.push_back(bottomRight);
      renderData.push_back(bottomLeft);
      // Texture coordinates
      renderData.push_back(Vector4(texCoord->GetXValue(0), texCoord->GetYValue(0), 0, 1));
      renderData.push_back(Vector4(texCoord->GetXValue(1), texCoord->GetYValue(0), 0, 1));
      renderData.push_back(Vector4(texCoord->GetXValue(1), texCoord->GetYValue(1), 0, 1));
      renderData.push_back(Vector4(texCoord->GetXValue(0), texCoord->GetYValue(1), 0, 1));
      // Colors (Assuming uniform color)
      renderData.push_back(color);
      renderData.push_back(color);
      renderData.push_back(color);
      renderData.push_back(color);
    }
    
    // TODO. vertex shaders are in clip space (-1,1)
    // Whereas our objects are in camera space.
    glUseProgram(program);
    glBindVertexArray(mVertexArrayObjectID);
    glEnableVertexAttribArray(0);
    if (renderData.size() > 0)
    {
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * renderData.size(), &renderData[0], GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector4), &renderData[1]);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector4), &renderData[2]);
      glDrawArrays(GL_QUADS, 0, renderData.size() / 3);
    }
    glDisableVertexAttribArray(0);
    glPopMatrix();

    // Reset to default texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
  int fullScreen = 0;
  if(aFullScreen)
    fullScreen = SDL_WINDOW_FULLSCREEN;
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

  glViewport(0, 0, aW, aH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, aW, aH, 0, 1, -1);

  glMatrixMode(GL_MODELVIEW);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

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
