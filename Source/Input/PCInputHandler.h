#ifndef __JFramework__PCInputHandler__
#define __JFramework__PCInputHandler__

#include "Common.h"
#include "InputHandler.h"

#if defined(_WIN32)
  #include "SDL.h"
#else
  #include <SDL2/SDL.h>
#endif

class PCInputHandler : public InputHandler
{
private:
  std::unordered_map<int, SDL_GameController*> mGameControllers;
  
public:
  PCInputHandler();
  virtual ~PCInputHandler();

  virtual void Update();
  virtual int GetInputCount();
  virtual void DeviceAdd(int const aId);
  virtual void DeviceRemove(int const aId);
  
  SDL_GameController* GetGameController(int const index) const;
};

#endif
