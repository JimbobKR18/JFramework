#ifndef __JFramework_Framebuffer_h_
#define __JFramework_Framebuffer_h_

#include "Common.h"

class GraphicsManager;

class Framebuffer
{
public:
  Framebuffer();
  virtual ~Framebuffer();
  
  // VIRTUAL GETTERS
  virtual int GetTextureID() const = 0;
  
  // VIRTUAL METHODS
  virtual void Generate(GraphicsManager *aManager) = 0;
  virtual void Bind() = 0;
  virtual void Unbind(int aDefaultFramebuffer) = 0;
  virtual void Draw(int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen) = 0;
};

#endif //__JFramework_Framebuffer_h_