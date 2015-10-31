#ifndef __JFramework_PCScreen_h_
#define __JFramework_PCScreen_h_

#include "Screen.h"
#include "Transform.h"
#if defined(_WIN32)
  #include "SDL.h"
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <SDL2/SDL.h>
#else
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL2/SDL.h>
#endif

class PCScreen : public Screen
{
  private:
    SDL_Window*   mWindow;
    SDL_GLContext mGLContext;

  public:
    PCScreen();
    PCScreen(int aW, int aH);
    virtual ~PCScreen();

    virtual void PreDraw();
    virtual void Draw(std::vector<Surface*> const &aObjects);
    virtual void DrawUI(std::vector<Surface*> const &aObjects);
    virtual void DebugDraw(std::vector<Surface*> const &aObjects);
    virtual void SwapBuffers();
    virtual void ChangeSize(int aW, int aH, bool aFullScreen);

  private:
    void AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition);
    bool PointIsOnScreen(Vector3 const &aPoint);
    bool BoxIsOnScreen(Vector3 const &aStart, Vector3 const &aEnd);
};

#endif
