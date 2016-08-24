#ifndef __JFramework_PCShaderScreen_h_
#define __JFramework_PCShaderScreen_h_

#include "Screen.h"
#include "Transform.h"
#include "PCShaderSurface.h"
#if defined(_WIN32)
  #include <GL\glew.h>
  #include "SDL.h"
#elif defined(__APPLE__)
  #include <GL/glew.h>
  #include <OpenGL/gl3.h>
  #include <OpenGL/gl3ext.h>
  #include <SDL2/SDL.h>
#else
  #include <GL/glew.h>
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
#endif

class PCShaderScreen : public Screen
{
private:
  SDL_Window*   mWindow;
  SDL_GLContext mGLContext;
  SDL_DisplayMode mDisplayMode;
  GLuint        mVertexBufferID;
  GLuint        mVertexArrayObjectID;

public:
  PCShaderScreen();
  PCShaderScreen(int aW, int aH, bool aFullScreen);
  virtual ~PCShaderScreen();

  virtual void PreDraw();
  virtual void Draw(std::vector<Surface*> const &aObjects);
  virtual void DrawUI(std::vector<Surface*> const &aObjects);
  virtual void DebugDraw(std::vector<Surface*> const &aObjects);
  virtual void SwapBuffers();
  virtual void ChangeSize(int aW, int aH, bool aFullScreen);
  virtual void AddOrEditProperty(Surface *aSurface, HashString const &aName, PropertyType const &aType, HashString const &aValue);

private:
  void AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition);
  bool PointIsOnScreen(Vector3 const &aPoint);
  bool BoxIsOnScreen(Vector3 const &aStart, Vector3 const &aEnd);
  void SetShaderProperties(PCShaderSurface *aSurface);
};

#endif
