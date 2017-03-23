#include "PCScreen.h"
#include "PhysicsObject.h"
#include "PCSurface.h"
#include "Constants.h"

#define VERTEX_ARRAYS

#ifdef _WIN32
  #include <Windows.h>
  #include <gl/gl.h>
  #undef SendMessage
  #undef GetYValue
  #undef PlaySound
#endif

PCScreen::PCScreen() : Screen()
{
  SDL_Init(SDL_INIT_EVERYTHING);
}

PCScreen::PCScreen(int aW, int aH, bool aFullScreen) : Screen(aW, aH, aFullScreen)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  mWindow = SDL_CreateWindow(Constants::GetString("GameTitle").ToCharArray(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aW, aH, 
                             SDL_WINDOW_OPENGL);
  mGLContext = SDL_GL_CreateContext(mWindow);
  SDL_GL_MakeCurrent(mWindow, mGLContext);
  SDL_GetDesktopDisplayMode(0, &mDisplayMode);
  ChangeSize(aW, aH, aFullScreen);
}

PCScreen::~PCScreen()
{
  SDL_GL_DeleteContext(mGLContext);
  SDL_Quit();
}

/**
 * @brief Draw debug lines around objects, physics boxes only.
 * @param aObjects
 */
void PCScreen::DebugDraw(std::vector<Surface*> const &aObjects)
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
      else if((*it)->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = GetWidth() * position.x;
        position.y = GetHeight() * position.y;
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
void PCScreen::PreDraw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}

/**
 * @brief Draw objects
 * @param aObjects
 */
void PCScreen::Draw(std::vector<Surface*> const &aObjects)
{
  // Camera position and size
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetHalfSize();
  Matrix33 viewMatrix = GetView().GetFinalTransform();
  
  // Must scale, rotate, then translate camera offset
  Vector3 cameraDiff = (viewMatrix * cameraPosition) - cameraSize;
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end;)
  {
    // Get the texture id of the surface
    GLuint texture = ((PCSurface*)(*it))->GetTextureID();
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    std::vector<Vector3> points, texcoords;
    std::vector<Vector4> colors;
    
    points.reserve(1024);
    texcoords.reserve(1024);
    colors.reserve(1024);
    
    // While other texture share the same texture id, draw them
    while(it != end &&
          (*it)->GetOwner()->GET<PCSurface>()->GetTextureID() == texture)
    {
      GameObject *owner = (*it)->GetOwner();
      PCSurface *surface = (PCSurface*)*it;
      Transform *transform = owner->GET<Transform>();
      
      // Gotta progress this somehow
      ++it;
      
      // Get transforms in local and world space.
      Matrix33 modelTransform = transform->GetRotation() * Matrix33(transform->GetScale());
      Vector3 position = transform->GetPosition();
      Vector3 size = transform->GetSize();
      
      TextureCoordinates *texCoord = surface->GetTextureData();
      Vector4 color = surface->GetColor();
      
      // If transform is a percentage of screen, convert.
      if((*it)->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = GetWidth() * position.x;
        position.y = GetHeight() * position.y;
      }
      
      // Move object based on its alignment
      AlignmentHelper(transform, size, position);
      
      // Get the basic coordinates for the quad
      Vector3 topLeft(-size.x, -size.y, 0);
      Vector3 topRight(size.x, -size.y, 0);
      Vector3 bottomRight(size.x, size.y, 0);
      Vector3 bottomLeft(-size.x, size.y, 0);
      
      // Model Transform
      topLeft = modelTransform * topLeft;
      topRight = modelTransform * topRight;
      bottomLeft = modelTransform * bottomLeft;
      bottomRight = modelTransform * bottomRight;
      topLeft += position;
      topRight += position;
      bottomRight += position;
      bottomLeft += position;
      
      // Apply camera transform
      if(surface->GetViewMode() == VIEW_ABSOLUTE)
      {
        topLeft = viewMatrix * topLeft;
        topRight = viewMatrix * topRight;
        bottomLeft = viewMatrix * bottomLeft;
        bottomRight = viewMatrix * bottomRight;
        topLeft -= cameraDiff;
        topRight -= cameraDiff;
        bottomLeft -= cameraDiff;
        bottomRight -= cameraDiff;
      }
      
      // Determine whether or not to draw (If any point is on screen or box collides)
      bool draw = PointIsOnScreen(topLeft) || PointIsOnScreen(topRight) || 
        PointIsOnScreen(bottomLeft) || PointIsOnScreen(bottomRight) ||
        BoxIsOnScreen(topLeft, bottomRight);
      if(!draw)
      {
        continue;
      }
      
      // Vertex points
      points.push_back(topLeft);
      points.push_back(topRight);
      points.push_back(bottomRight);
      points.push_back(bottomLeft);
      // Texture coordinates
      texcoords.push_back(Vector3(texCoord->GetXValue(0), texCoord->GetYValue(0), 0));
      texcoords.push_back(Vector3(texCoord->GetXValue(1), texCoord->GetYValue(0), 0));
      texcoords.push_back(Vector3(texCoord->GetXValue(1), texCoord->GetYValue(1), 0));
      texcoords.push_back(Vector3(texCoord->GetXValue(0), texCoord->GetYValue(1), 0));
      // Colors (Assuming uniform color)
      colors.push_back(color);
      colors.push_back(color);
      colors.push_back(color);
      colors.push_back(color);
    }
    
    // Pointers and draw (there has to be something to draw though
    if (points.size() > 0)
    {
      glVertexPointer(3, GL_FLOAT, sizeof(Vector3), &points[0]);
      glTexCoordPointer(3, GL_FLOAT, sizeof(Vector3), &texcoords[0]);
      glColorPointer(4, GL_FLOAT, sizeof(Vector4), &colors[0]);
      glDrawArrays(GL_QUADS, 0, points.size());
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glPopMatrix();

    // Reset to default texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

/**
 * @brief Draw UI objects
 * @param aObjects
 */
void PCScreen::DrawUI(std::vector<Surface*> const &aObjects)
{
  Draw(aObjects);
}

/**
 * @brief Swap buffers
 */
void PCScreen::SwapBuffers()
{
  SDL_GL_SwapWindow(mWindow);
}

/**
 * @brief Set clear color.
 * @param aClearColor Color.
 */
void PCScreen::SetClearColor(Vector4 const &aClearColor)
{
  glClearColor(aClearColor.x, aClearColor.y, aClearColor.z, aClearColor.w);
}

/**
 * @brief Change window size
 * @param aW Width
 * @param aH Height
 * @param aFullScreen
 */
void PCScreen::ChangeSize(int aW, int aH, bool aFullScreen)
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

  glShadeModel(GL_SMOOTH);

  glLoadIdentity();
}

/**
 * @brief Align objects
 * @param aTransform Transform of object
 * @param aSize Object size
 * @param aPosition Object position
 */
void PCScreen::AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition)
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
bool PCScreen::PointIsOnScreen(Vector3 const &aPoint)
{
  return aPoint.x < GetWidth() && aPoint.y < GetHeight() &&
         aPoint.x > 0 && aPoint.y > 0;
}

/**
 * @brief Check if box is on screen
 * @param aStart Top left of box
 * @param aEnd Bottom right of box
 */
bool PCScreen::BoxIsOnScreen(Vector3 const &aStart, Vector3 const &aEnd)
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
