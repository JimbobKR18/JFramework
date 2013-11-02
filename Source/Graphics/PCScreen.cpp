#include "PCScreen.h"
#include "Transform.h"
#include "PCSurface.h"

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

void PCScreen::Draw(std::vector<Surface*> const &aObjects)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  // Camera position and size
  Vector3 cameraPosition = GetView().GetPosition();
  Vector3 cameraSize = GetView().GetSize();
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != aObjects.end();)
  {
    // Get the texture id of the surface
    GLuint texture = (*it)->GetOwner()->GET<PCSurface>()->GetTextureID();
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glPushMatrix();
    glBegin(GL_QUADS);
    
    // while other texture share the same texture id, draw them
    while(it != aObjects.end() &&
          (*it)->GetOwner()->GET<PCSurface>()->GetTextureID() == texture)
    {
      GameObject *owner = (*it)->GetOwner();
      
      // Gotta progress this somehow
      ++it;
      
      // Get position and size
      Vector3 position = owner->GET<Transform>()->GetPosition();
      Vector3 size = owner->GET<Transform>()->GetSize();
      
      TextureCoordinates *texCoord = owner->GET<PCSurface>()->GetTextureData();
      
      // Get positions relative to the camera
      float xPosition = position.x;
      float yPosition = position.y;
      float zPosition = position.z;

      if(owner->GET<PCSurface>()->GetViewMode() == VIEW_ABSOLUTE)
      {
        xPosition -= (cameraPosition.x - cameraSize.x / 2.0f);
        yPosition -= (cameraPosition.y - cameraSize.y / 2.0f);
        zPosition -= (cameraPosition.z - cameraSize.z / 2.0f);
      }
      
      // TODO FIX THIS
      if(xPosition - size.x > GetWidth() || yPosition - size.y > GetHeight() ||
         xPosition + size.x < GetWidth()/2 || yPosition + size.y < GetHeight()/2)
      {
        continue;
      }
      
      // Actually draw the object
      glTexCoord2f(texCoord->GetXValue(0), texCoord->GetYValue(0));
      glVertex3f(xPosition - size.x, yPosition - size.y, zPosition);
      glTexCoord2f(texCoord->GetXValue(1), texCoord->GetYValue(0));
      glVertex3f(xPosition + size.x, yPosition - size.y, zPosition);
      glTexCoord2f(texCoord->GetXValue(1), texCoord->GetYValue(1));
      glVertex3f(xPosition + size.x, yPosition + size.y, zPosition);
      glTexCoord2f(texCoord->GetXValue(0), texCoord->GetYValue(1));
      glVertex3f(xPosition - size.x, yPosition + size.y, zPosition);
    }
    
    glEnd();
    glPopMatrix();
    
    // Reset to default texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
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

  glLoadIdentity();
}
