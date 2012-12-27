//
//  ControllerManager.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/26/12.
//
//

#include "Common.h"
#include "ControllerManager.h"

ControllerManager::ControllerManager(GameApp* aApp) : Manager(aApp, "ControllerManager")
{
  
}
ControllerManager::~ControllerManager()
{
  ClearControllers();
}

void ControllerManager::AddController(Controller *aController)
{
  mControllers.push_back(aController);
}
void ControllerManager::RemoveController(Controller *aController)
{
  for(std::vector<Controller*>::iterator it = mControllers.begin();
      it != mControllers.end(); ++it)
  {
    if(*it == aController)
    {
      mControllers.erase(it);
      delete *it;
      break;
    }
  }
}
void ControllerManager::ClearControllers()
{
  for(std::vector<Controller*>::iterator it = mControllers.begin();
      it != mControllers.end(); ++it)
  {
    RemoveController(*it);
  }
  mControllers.clear();
}

void ControllerManager::Update()
{
  
}
void ControllerManager::SendMessage(Message const &aMessage)
{
  for(std::vector<Controller*>::iterator it = mControllers.begin();
      it != mControllers.end(); ++it)
  {
    //(*it)->ReceiveMessage(aMessage);
  }
}