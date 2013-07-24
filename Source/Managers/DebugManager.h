/*
 * DebugManager.h
 *
 *  Created on: Feb 16, 2013
 *      Author: jimbob
 */

#ifndef DEBUGMANAGER_H_
#define DEBUGMANAGER_H_

#include "Manager.h"
#include "AntTweakBar.h"
#include "Common.h"

#if defined(_WIN32) || defined(__APPLE__)
  #include "SDL.h"
#else
  #include <SDL/SDL.h>
#endif

class DebugManager : public Manager
{
private:
  std::map<std::string, TwBar*>   mTweakBars;
public:
  DebugManager(GameApp *aApp);
  ~DebugManager();

  void                AddTweakBar(std::string const &aName);
  void                AddInt(std::string const &aTweakBarName,
                             std::string const &aVarName,
                             int *aVariable);
  void                AddBool(std::string const &aTweakBarName,
                               std::string const &aVarName,
                               bool *aVariable);
  void                AddFloat(std::string const &aTweakBarName,
                               std::string const &aVarName,
                               float *aVariable);
  int                 HandleEvent(SDL_Event &event);

  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "DebugManager";}
};

#endif /* DEBUGMANAGER_H_ */
