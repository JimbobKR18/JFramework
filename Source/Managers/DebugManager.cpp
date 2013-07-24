/*
 * DebugManager.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: jimbob
 */

#include "DebugManager.h"
#include "GraphicsManager.h"

DebugManager::DebugManager(GameApp *aApp) : Manager(aApp, "DebugManager")
{
  //TwInit(TW_OPENGL, NULL);
  //Screen *screen = GetOwningApp()->GET<GraphicsManager>()->GetScreen();
  //TwWindowSize(screen->GetWidth(),screen->GetHeight());
}

DebugManager::~DebugManager()
{
  //TwTerminate();
  mTweakBars.clear();
}

void DebugManager::AddTweakBar(std::string const &aName)
{
  //TwBar *bar = TwNewBar(aName.c_str());
  //mTweakBars.insert(std::pair<std::string, TwBar*>(aName, bar));
}
void DebugManager::AddInt(std::string const &aTweakBarName,
                          std::string const &aVarName,
                          int *aVariable)
{
  //TwAddVarRW(mTweakBars[aTweakBarName], aVarName.c_str(), TW_TYPE_INT32, aVariable, "");
}
void DebugManager::AddBool(std::string const &aTweakBarName,
                           std::string const &aVarName,
                           bool *aVariable)
{
  //TwAddVarRW(mTweakBars[aTweakBarName], aVarName.c_str(), TW_TYPE_BOOLCPP, aVariable, "");
}
void DebugManager::AddFloat(std::string const &aTweakBarName,
                            std::string const &aVarName,
                            float *aVariable)
{
  //TwAddVarRW(mTweakBars[aTweakBarName], aVarName.c_str(), TW_TYPE_FLOAT, aVariable, "");
}
int DebugManager::HandleEvent(SDL_Event &event)
{
  return 0;
  //return TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
}

void DebugManager::Update()
{
  //TwDraw();
}
void DebugManager::SendMessage(Message const &aMessage)
{
}
void DebugManager::ProcessDelayedMessage(Message *aMessage)
{
}
