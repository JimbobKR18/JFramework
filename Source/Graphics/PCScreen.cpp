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

PCScreen::PCScreen(int aW, int aH) : Screen(aW, aH)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  mWindow = SDL_CreateWindow(Constants::GetString("GameTitle").ToCharArray(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, aW, aH, 
                             SDL_GL_DOUBLEBUFFER | SDL_WINDOW_OPENGL);
  mGLContext = SDL_GL_CreateContext(mWindow);
  ChangeSize(aW, aH, Constants::GetBoolean("FullScreen"));
}

PCScreen::~PCScreen()
{
  SDL_GL_DeleteContext(mGLContext);
  SDL_Quit();
}

void PCScreen::DebugDraw(std::vector<Surface*> const &aObjects)
{
  // Draw debug hitboxes for objects in environment, requires PhysicsObject
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetSize();
  Vector3 cameraDiff = cameraPosition - (cameraSize / 2.0f);
  
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

void PCScreen::PreDraw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}

void PCScreen::Draw(std::vector<Surface*> const &aObjects)
{
  // Camera position and size
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetHalfSize();
  Vector3 cameraDiff = cameraPosition - cameraSize;
  Vector3 cameraScale = GetView().GetScale();
  Matrix33 cameraRotation = GetView().GetRotation();
  
  // Camera scaling
  cameraDiff.x *= cameraScale.x;
  cameraDiff.y *= cameraScale.y;
  cameraDiff.z *= cameraScale.z;
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end;)
  {
    // Get the texture id of the surface
    GLuint texture = (*it)->GetOwner()->GET<PCSurface>()->GetTextureID();
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    std::vector<Vector3> points, texcoords;
    std::vector<Vector4> colors;
    
    // While other texture share the same texture id, draw them
    while(it != end &&
          (*it)->GetOwner()->GET<PCSurface>()->GetTextureID() == texture)
    {
      GameObject *owner = (*it)->GetOwner();
      PCSurface *surface = owner->GET<PCSurface>();
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
      size.x *= cameraScale.x;
      size.y *= cameraScale.y;
      size.z *= cameraScale.z;
      
      // Camera rotation
      position = cameraRotation * position;

      // Camera translation
      if(surface->GetViewMode() == VIEW_ABSOLUTE)
      {
        position -= cameraDiff;
      }

      // Logic too long, put into helper
      AlignmentHelper(transform, size, position.x, position.y, position.z);

      // Scaling
      size.x *= scale.x;
      size.y *= scale.y;
      size.z *= scale.z;
      
      // Get the basic coordinates for the quad
      Vector3 topLeft = Vector3(-size.x, -size.y, 0);
      Vector3 topRight = Vector3(size.x, -size.y, 0);
      Vector3 bottomRight = Vector3(size.x, size.y, 0);
      Vector3 bottomLeft = Vector3(-size.x, size.y, 0);
      
      // Rotate
      topLeft = rotation * topLeft;
      topRight = rotation * topRight;
      bottomLeft = rotation * bottomLeft;
      bottomRight = rotation * bottomRight;

      // Translate
      topLeft += position;
      topRight += position;
      bottomRight += position;
      bottomLeft += position;
      
      // Determine whether or not to draw
      bool draw = topLeft.x < GetWidth() && topLeft.y < GetHeight() &&
                  bottomRight.x > 0 && bottomRight.y > 0;
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

void PCScreen::DrawUI(std::vector<Surface*> const &aObjects)
{
  Draw(aObjects);
}

void PCScreen::SwapBuffers()
{
  SDL_GL_SwapWindow(mWindow);
}

void PCScreen::ChangeSize(int aW, int aH, bool aFullScreen)
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
}

void PCScreen::AlignmentHelper(Transform *aTransform, Vector3 const &aSize, float &aXPosition, float &aYPosition, float &aZPosition)
{
  X_ALIGNMENT xAlign = aTransform->GetXAlignment();
  Y_ALIGNMENT yAlign = aTransform->GetYAlignment();
  Z_ALIGNMENT zAlign = aTransform->GetZAlignment();

  switch(xAlign)
  {
  case X_ALIGN_LEFT:
    aXPosition += aSize.x;
    break;
  case X_ALIGN_RIGHT:
    aXPosition -= aSize.x;
    break;
  default:
    break;
  }

  switch(yAlign)
  {
  case Y_ALIGN_TOP:
    aYPosition += aSize.y;
    break;
  case Y_ALIGN_BOTTOM:
    aYPosition -= aSize.y;
    break;
  default:
    break;
  }

  switch(zAlign)
  {
  case Z_ALIGN_FRONT:
    aZPosition += aSize.z;
    break;
  case Z_ALIGN_BACK:
    aZPosition -= aSize.z;
    break;
  default:
    break;
  }
}
