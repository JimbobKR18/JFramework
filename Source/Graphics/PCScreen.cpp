#include "PCScreen.h"
#include "Transform.h"
#include "PCSurface.h"

PCScreen::PCScreen() : Screen()
{
  SDL_Init(SDL_INIT_EVERYTHING);
}

PCScreen::PCScreen(int aW, int aH) : Screen(aW, aH)
{
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

  for(std::vector<Surface*>::const_iterator it = aObjects.begin(); it != aObjects.end(); ++it)
  {
    GameObject *owner = (*it)->GetOwner();
    Vector3 cameraPosition = GetView().GetPosition();
    Vector3 position = owner->GET<Transform>()->GetPosition();
    Vector3 size = owner->GET<Transform>()->GetSize();
    GLuint texture = owner->GET<PCSurface>()->GetTexID();

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
      glTexCoord2i(0, 0); glVertex3f(position.x - size.x, position.y - size.y, 0);
      glTexCoord2i(1, 0); glVertex3f(position.x + size.x, position.y - size.y, 0);
      glTexCoord2i(1, 1); glVertex3f(position.x + size.x, position.y + size.y, 0);
      glTexCoord2i(0, 1); glVertex3f(position.x - size.x, position.y + size.y, 0);
    glEnd();
  }

  SDL_GL_SwapBuffers();
}

void PCScreen::ChangeSize(int aW, int aH)
{
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
