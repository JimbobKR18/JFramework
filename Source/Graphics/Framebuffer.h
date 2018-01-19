#ifndef __JFramework_Framebuffer_h_
#define __JFramework_Framebuffer_h_

#include "Common.h"
#include "GraphicsManager.h"

class Framebuffer
{
public:
  Framebuffer();
  virtual ~Framebuffer();
  
  virtual void Generate(GraphicsManager *aManager) = 0;
  virtual void Bind() = 0;
  virtual void Draw(int aDefaultFramebuffer, int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen) = 0;
};

#endif //__JFramework_Framebuffer_h_