#include "PCScreen.h"
#include "PhysicsObject.h"
#include "PCSurface.h"

#define VERTEX_ARRAYS

PCScreen::PCScreen() : Screen()
{
  SDL_Init(SDL_INIT_EVERYTHING);
}

PCScreen::PCScreen(int aW, int aH) : Screen(aW, aH)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  mSurface = SDL_SetVideoMode(aW, aH, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);
  ChangeSize(aW, aH);
}

PCScreen::~PCScreen()
{
  SDL_Quit();
}

void PCScreen::DebugDraw(std::vector<Surface*> const &aObjects)
{
  // Draw debug hitboxes for objects in environment, requires PhysicsObject
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetSize();
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end; ++it)
  {
    GameObject *obj = (*it)->GetOwner();

    if(obj->HAS<PhysicsObject>())
    {
      Transform *transform = obj->GET<Transform>();
      Vector3 position = transform->GetPosition();
      Vector3 size = transform->GetSize();

      // Get positions relative to the camera
      float xPosition = position.x;
      float yPosition = position.y;
      float zPosition = position.z;

      if((*it)->GetViewMode() == VIEW_ABSOLUTE)
      {
        xPosition -= (cameraPosition.x - cameraSize.x / 2.0f);
        yPosition -= (cameraPosition.y - cameraSize.y / 2.0f);
        zPosition -= (cameraPosition.z - cameraSize.z / 2.0f);
      }

      glLoadIdentity();
      glPushMatrix();
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(xPosition - size.x, yPosition - size.y, zPosition);
      glVertex3f(xPosition + size.x, yPosition - size.y, zPosition);
      glVertex3f(xPosition + size.x, yPosition + size.y, zPosition);
      glVertex3f(xPosition - size.x, yPosition + size.y, zPosition);
      glVertex3f(xPosition - size.x, yPosition - size.y, zPosition);
      glEnd();
      glPopMatrix();
    }
  }
}

void PCScreen::Draw(std::vector<Surface*> const &aObjects)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  // Camera position and size
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetSize();
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::vector<Surface*>::const_iterator end = aObjects.end();
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != end;)
  {
    // Get the texture id of the surface
    GLuint texture = (*it)->GetOwner()->GET<PCSurface>()->GetTextureID();
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glPushMatrix();

#ifdef VERTEX_ARRAYS
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    std::vector<Vector3> points, texcoords;
    std::vector<Vector4> colors;
#else
    glBegin(GL_QUADS);
#endif
    
    // While other texture share the same texture id, draw them
    while(it != end &&
          (*it)->GetOwner()->GET<PCSurface>()->GetTextureID() == texture)
    {
      GameObject *owner = (*it)->GetOwner();
      PCSurface *surface = owner->GET<PCSurface>();
      Transform *transform = owner->GET<Transform>();
      
      // Gotta progress this somehow
      ++it;
      
      // Get position and size
      Vector3 position = transform->GetPosition();
      Vector3 size = transform->GetSize();
      
      TextureCoordinates *texCoord = surface->GetTextureData();
      Vector4 color = surface->GetColor();
      
      // Get positions relative to the camera
      float xPosition = position.x;
      float yPosition = position.y;
      float zPosition = position.z;

      if(surface->GetViewMode() == VIEW_ABSOLUTE)
      {
        xPosition -= (cameraPosition.x - cameraSize.x / 2.0f);
        yPosition -= (cameraPosition.y - cameraSize.y / 2.0f);
        zPosition -= (cameraPosition.z - cameraSize.z / 2.0f);
      }

      // Logic too long, put into helper
      AlignmentHelper(transform, size, xPosition, yPosition, zPosition);

      bool draw = xPosition - size.x < GetWidth() && yPosition - size.y < GetHeight() &&
                  xPosition + size.x > 0 && yPosition + size.y > 0;
      if(!draw)
      {
        continue;
      }
      
#ifdef VERTEX_ARRAYS
      // Vertex points
      points.push_back(Vector3(xPosition - size.x, yPosition - size.y, zPosition));
      points.push_back(Vector3(xPosition + size.x, yPosition - size.y, zPosition));
      points.push_back(Vector3(xPosition + size.x, yPosition + size.y, zPosition));
      points.push_back(Vector3(xPosition - size.x, yPosition + size.y, zPosition));
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
#else
      // Actually draw the object
      glColor4f(color.x, color.y, color.z, color.w);
      glTexCoord2f(texCoord->GetXValue(0), texCoord->GetYValue(0));
      glVertex3f(xPosition - size.x, yPosition - size.y, zPosition);
      glTexCoord2f(texCoord->GetXValue(1), texCoord->GetYValue(0));
      glVertex3f(xPosition + size.x, yPosition - size.y, zPosition);
      glTexCoord2f(texCoord->GetXValue(1), texCoord->GetYValue(1));
      glVertex3f(xPosition + size.x, yPosition + size.y, zPosition);
      glTexCoord2f(texCoord->GetXValue(0), texCoord->GetYValue(1));
      glVertex3f(xPosition - size.x, yPosition + size.y, zPosition);
#endif
    }
    
#ifdef VERTEX_ARRAYS
    // Pointers and draw
    glVertexPointer(3, GL_FLOAT, sizeof(Vector3), &points[0]);
    glTexCoordPointer(3, GL_FLOAT, sizeof(Vector3), &texcoords[0]);
    glColorPointer(4, GL_FLOAT, sizeof(Vector4), &colors[0]);
    glDrawArrays(GL_QUADS, 0, points.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
#else
    glEnd();
#endif
    glPopMatrix();

    // Reset to default texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void PCScreen::SwapBuffers()
{
  SDL_GL_SwapBuffers();
}

void PCScreen::ChangeSize(int aW, int aH)
{
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

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
