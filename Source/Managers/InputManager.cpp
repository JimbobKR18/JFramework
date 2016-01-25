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

/**
 * @brief Keeps track of input
 * @param aInput Input type
 * @param aLocation Lcoation in screen space where input happened.
 */
void InputManager::AddInput(HashString const &aInput, Vector3 const &aLocation)
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

/**
 * @brief Removes input
 * @param aInput Input type
 */
void InputManager::RemoveInput(HashString const &aInput)
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

/**
 * @brief Clear all inputs
 */
void InputManager::ClearInputs()
{
  mInputs.clear();
}

/**
 * @brief Method to start accepting inputs.
 */
void InputManager::AcceptInputs()
{
  mAcceptInput = true;
  ClearInputs();
}

/**
 * @brief Method to start declining inputs.
 */
void InputManager::DeclineInputs()
{
  mAcceptInput = false;
  ClearInputs();
}

/**
 * @brief Check to see if accepting input
 */
bool InputManager::AcceptingInputs()
{
  return mAcceptInput;
}

/**
 * @brief Update loop
 */
void InputManager::Update()
{
  for(InputIT it = mInputs.begin(); it != mInputs.end(); ++it)
  {
    GetOwningApp()->SendMessageDelayed(new InputMessage(it->mInput, it->mLocation));
  }
}

/**
 * @brief Send message out.
 * @param aMessage Message to send.
 */
void InputManager::SendMessage(Message const &aMessage)
{
}

/**
 * @brief Receive message
 * @param aMessage Message to read.
 */
void InputManager::ProcessDelayedMessage(Message *aMessage)
{
}
