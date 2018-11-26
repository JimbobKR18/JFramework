//
//  ControllerManager.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/26/12.
//
//

#include "Common.h"
#include "ControllerManager.h"
#include "ControllerFactory.h"
#include "ControllerChangeMessage.h"

unsigned const ControllerManager::sUID = Common::StringHashFunction("ControllerManager");
ControllerManager::ControllerManager(GameApp* aApp) : Manager(aApp, "ControllerManager", ControllerManager::sUID),
  mControllers(), mFactory(nullptr)
{
}
ControllerManager::~ControllerManager()
{
  ClearControllers();
}

/**
 * @brief Set controller generation factory.
 * @param aFactory Factory to generate controllers.
 */
void ControllerManager::SetControllerFactory(ControllerFactory* aFactory)
{
  if(mFactory)
    delete mFactory;
  
  mFactory = aFactory;
}

/**
 * @brief Create controller using factory.
 * @param aName Name of controller type.
 * @return New controller.
 */
Controller* ControllerManager::CreateController(HashString const &aName, ParserNode* const aData)
{
  Controller* controller = mFactory->CreateController(this, aName, aData);
  AddController(controller);
  return controller;
}

/**
 * @brief Delete Controller
 * @param aController
 */
void ControllerManager::DeleteController(Controller *aController)
{
  RemoveController(aController);
  delete aController;
}

/**
 * @brief Add controller to list
 * @param aController
 */
void ControllerManager::AddController(Controller *aController)
{
  // Check to see if object is in our list
  mControllers.insert(aController);
}

/**
 * @brief Remove controller to list
 * @param aController
 */
void ControllerManager::RemoveController(Controller *aController)
{
  mControllers.erase(aController);
}

/**
 * @brief Set pause state for all controllers
 * @param aPaused
 */
void ControllerManager::SetControllersPauseState(bool const aPaused)
{
  for(ControllerIT it = mControllers.begin(); it != mControllers.end(); ++it)
  {
    (*it)->SetPaused(aPaused);
  }
}

/**
 * @brief Clear all controllers (deletes them too)
 */
void ControllerManager::ClearControllers()
{
  for(ControllerIT it = mControllers.begin(); it != mControllers.end(); ++it)
  {
    RemoveController(*it);
  }
  mControllers.clear();
}

/**
 * @brief Update loop
 */
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

/**
 * @brief Send out message
 * @param aMessage
 */
void ControllerManager::SendMessage(Message const &aMessage)
{
}

/**
 * @brief Receive messages
 * @param aMessage
 */
void ControllerManager::ProcessDelayedMessage(Message *aMessage)
{
  mDelayedMessages.push_back(aMessage);
}

/**
 * @brief Make this manager visible in LUA.
 */
void ControllerManager::SerializeLUA()
{
  SLB::Class<ControllerManager>("ControllerManager").inherits<Manager>();
}