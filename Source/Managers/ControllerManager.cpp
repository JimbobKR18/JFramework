//
//  ControllerManager.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/26/12.
//
//

#include "Common.h"
#include "ControllerManager.h"
#include "ControllerChangeMessage.h"

unsigned const ControllerManager::sUID = Common::StringHashFunction("ControllerManager");
ControllerManager::ControllerManager(GameApp* aApp) : Manager(aApp, "ControllerManager", ControllerManager::sUID)
{
}
ControllerManager::~ControllerManager()
{
  ClearControllers();
}

void ControllerManager::AddController(Controller *aController)
{
  // Check to see if object is in our list
  for(ControllerIT it = mControllers.begin(); it != mControllers.end(); ++it)
  {
    if(*it == aController)
    {
      return;
    }
  }
  
  mControllers.push_back(aController);
}
void ControllerManager::RemoveController(Controller *aController)
{
  for(ControllerIT it = mControllers.begin(); it != mControllers.end(); ++it)
  {
    if(*it == aController)
    {
      mControllers.erase(it);
      break;
    }
  }
}
void ControllerManager::DeleteController(Controller *aController)
{
  RemoveController(aController);
  delete aController;
}
void ControllerManager::SetControllersPauseState(bool const aPaused)
{
  for(ControllerIT it = mControllers.begin(); it != mControllers.end(); ++it)
  {
    (*it)->SetPaused(aPaused);
  }
}
void ControllerManager::ClearControllers()
{
  for(ControllerIT it = mControllers.begin(); it != mControllers.end(); ++it)
  {
    RemoveController(*it);
  }
  mControllers.clear();
}

void ControllerManager::Update()
{
  for(MessageIT it = mDelayedMessages.begin(); it != mDelayedMessages.end(); ++it)
  {
    ControllerChangeMessage *msg = (ControllerChangeMessage*)*it;
    msg->mCurrentOwner->RemoveComponent(msg->mController, false);
    if(msg->mNewOwner)
      msg->mNewOwner->AddComponent(msg->mController);
    delete *it;
  }
  mDelayedMessages.clear();
}
void ControllerManager::SendMessage(Message const &aMessage)
{
  
}
void ControllerManager::ProcessDelayedMessage(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
}
