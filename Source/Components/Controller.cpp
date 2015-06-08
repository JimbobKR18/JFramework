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
Controller::Controller(int const &aUID) : Component(aUID)
{
}

Controller::Controller() : Component(Controller::sUID)
{
}

Controller::~Controller()
{
  GetOwner()->GetManager()->GetOwningApp()->GET<ControllerManager>()->RemoveController(this);
}
