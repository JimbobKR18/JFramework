/*
 * DebugManager.h
 *
 *  Created on: Feb 16, 2013
 *      Author: jimbob
 */

#ifndef DEBUGMANAGER_H_
#define DEBUGMANAGER_H_

#include "Manager.h"
#include "Common.h"

#if defined(_WIN32)
  #include "SDL.h"
#elif defined(__APPLE__)
  #include <SDL2/SDL.h>
#else
  #include <SDL2/SDL.h>
#endif

class DebugManager : public Manager
{
private:
public:
  DebugManager(GameApp *aApp);
  ~DebugManager();

  int                 HandleEvent(SDL_Event &event);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "DebugManager";}
};

#endif /* DEBUGMANAGER_H_ */
