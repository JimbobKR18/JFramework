//
//  InputManager.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/27/12.
//
//

#include "InputManager.h"
#include "InputMessage.h"

unsigned const InputManager::sUID = Common::StringHashFunction("InputManager");
InputManager::InputManager(GameApp *aApp) : Manager(aApp, "InputManager", InputManager::sUID), mAcceptInput(true)
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
  GetOwningApp()->SendMessageDelayed(new InputMessage(aInput + std::string("_Down"), aLocation));
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
  GetOwningApp()->SendMessageDelayed(new InputMessage(aInput + std::string("_Up"), Vector3()));
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
