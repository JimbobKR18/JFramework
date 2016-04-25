//
//  Controller.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/25/12.
//
//

#include "Controller.h"
#include "ControllerManager.h"
#include "ObjectManager.h"

int const Controller::sUID = Common::StringHashFunction("Controller");

// PROTECTED
Controller::Controller(int const &aUID) : Component(aUID), mPaused(false)
{
}

Controller::Controller() : Component(Controller::sUID), mPaused(false)
{
  assert(!"Default Controller constructor called, are you sure you meant to do this?");
}

Controller::~Controller()
{
  GetOwner()->GetManager()->GetOwningApp()->GET<ControllerManager>()->RemoveController(this);
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
