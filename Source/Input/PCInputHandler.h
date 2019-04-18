#ifndef __JFramework__PCInputHandler__
#define __JFramework__PCInputHandler__

#include "Common.h"
#include "InputHandler.h"

#if defined(_WIN32)
  #include "SDL.h"
#else
  #include <SDL2/SDL.h>
#endif

struct JoypadController
{
  SDL_GameController* mController;
  SDL_JoystickID mInstanceId;
  
  JoypadController() : mController(nullptr), mInstanceId(-1)
  {
  }
  virtual ~JoypadController()
  {
  }
};

class PCInputHandler : public InputHandler
{
private:
  std::unordered_map<int, JoypadController*> mGameControllers;
  
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
