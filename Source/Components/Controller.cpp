//
//  Controller.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/25/12.
//
//

#include "Controller.h"
#include "ControllerManager.h"

HashType const Controller::sUID = Common::StringHashFunction("Controller");

// PROTECTED
Controller::Controller(ControllerManager *aManager, HashType const &aUID) : Component(aUID), mManager(aManager), mPaused(false)
{
  aManager->AddController(this);
}

Controller::Controller() : Component(Controller::sUID), mPaused(false)
{
  assert(!"Default Controller constructor called, are you sure you meant to do this?");
}

Controller::~Controller()
{
  mManager = nullptr;
}

/**
 * @brief Get manager.
 * @return Manager.
 */
ControllerManager* Controller::GetManager() const
{
  return mManager;
}

/**
 * @brief Set pause state (if true, controller cannot move, is the idea)
 * @param aPaused Pause state
 */
void Controller::SetPaused(bool const aPaused)
{
  mPaused = aPaused;
}

/**
 * @brief Get pause state
 * @return Pause state
 */
bool Controller::GetPaused() const
{
  return mPaused;
}
