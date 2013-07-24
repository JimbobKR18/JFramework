//
//  InputManager.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/27/12.
//
//

#include "InputManager.h"
#include "InputMessage.h"

InputManager::InputManager(GameApp *aApp) : Manager(aApp, "InputManager"), mAcceptInput(true)
{
}
InputManager::~InputManager()
{
  mInputs.clear();
}

void InputManager::AddInput(std::string const &aInput, Vector3 const &aLocation)
{
  if(!mAcceptInput)
    return;

  // Check to see if object is in our list
  for(InputIT it = mInputs.begin(); it != mInputs.end(); ++it)
  {
    if(it->mInput == aInput)
    {
      return;
    }
  }
  
  mInputs.insert(InputInfo(aInput, aLocation));
}
void InputManager::RemoveInput(std::string const &aInput)
{
  for(InputIT it = mInputs.begin(); it != mInputs.end(); ++it)
  {
    if(it->mInput == aInput)
    {
      mInputs.erase(it);
      break;
    }
  }
}
void InputManager::ClearInputs()
{
  mInputs.clear();
}

void InputManager::AcceptInputs()
{
  mAcceptInput = true;
  ClearInputs();
}
void InputManager::DeclineInputs()
{
  mAcceptInput = false;
  ClearInputs();
}
bool InputManager::AcceptingInputs()
{
  return mAcceptInput;
}

// Derived from Manager
void InputManager::Update()
{
  for(InputIT it = mInputs.begin(); it != mInputs.end(); ++it)
  {
    GetOwningApp()->SendMessageDelayed(new InputMessage(it->mInput, it->mLocation));
  }
}
void InputManager::SendMessage(Message const &aMessage)
{
}

void InputManager::ProcessDelayedMessage(Message *aMessage)
{
}
