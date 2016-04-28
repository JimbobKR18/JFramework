#ifndef __JFramework__InputHandler__
#define __JFramework__InputHandler__

#include "Common.h"

class InputHandler
{
public:
  InputHandler();
  virtual ~InputHandler();
  
  virtual void Update() = 0;
};

#endif