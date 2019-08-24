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
#include "ObjectDeleteMessage.h"
#include "ObjectCreateMessage.h"

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
  static HashType const sUID;
  std::map<size_t, int> mInvalidMemory;
public:
  DebugManager(GameApp *aApp);
  ~DebugManager();

  int                 HandleEvent(SDL_Event &event);
  void                HandleCreate(ObjectCreateMessage *aMsg);
  void                HandleDelete(ObjectDeleteMessage *aMsg);
  void                LogException(std::exception const &aException);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "DebugManager";}
  static HashType     GetUID() {return sUID;}
};

#endif /* DEBUGMANAGER_H_ */
