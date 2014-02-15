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

// PROTECTED
Controller::Controller(std::string const &aName) : Component(aName)
{
}

Controller::Controller() : Component("Controller")
{
}

Controller::~Controller()
{
  GetOwner()->GetManager()->GetOwningApp()->GET<ControllerManager>()->RemoveController(this);
}
