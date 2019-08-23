//
//  InputManager.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/27/12.
//
//

#include "InputManager.h"
#include "InputMessage.h"
#include "PCInputHandler.h"

unsigned const InputManager::sUID = Common::StringHashFunction("InputManager");

InputManager::InputManager(GameApp *aApp) : Manager(aApp, "InputManager", InputManager::sUID), mAcceptInput(true)
{
  #if !defined(IOS) && !defined(ANDROID)
    mHandler = new PCInputHandler();
  #else
  #endif
  mHandler->Update();
}

InputManager::~InputManager()
{
  mInputs.clear();
  delete mHandler;
}

/**
 * @brief Keeps track of input
 * @param aInput Input type
 * @param aLocation Lcoation in screen space where input happened.
 */
void InputManager::AddInput(HashString const &aInput, Vector3 const &aLocation, int const aId, bool const aSingleFrame)
{
  if(!mAcceptInput)
    return;

  // Check to see if object is in our list
  for(InputIT it = mInputs.begin(); it != mInputs.end(); ++it)
  {
    if((*it)->mInput == aInput && (*it)->mId == aId)
    {
      return;
    }
  }
  
  mInputs.push_back(new InputInfo(aInput, aLocation, aId, aSingleFrame));
  GetOwningApp()->SendMessageDelayed(new InputMessage(aInput + std::string("_Down"), aLocation, aId));
}

/**
 * @brief Removes input
 * @param aInput Input type
 */
void InputManager::RemoveInput(HashString const &aInput, int const aId)
{
  for(InputIT it = mInputs.begin(); it != mInputs.end(); ++it)
  {
    if((*it)->mInput == aInput && (*it)->mId == aId)
    {
      delete *it;
      mInputs.erase(it);
      break;
    }
  }
  GetOwningApp()->SendMessageDelayed(new InputMessage(aInput + std::string("_Up"), Vector3(), aId));
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
 * @brief Get input handler
 * @return Input handler
 */
InputHandler* InputManager::GetInputHandler() const
{
  return mHandler;
}

/**
 * @brief Get total input device count
 * @return Device count
 */
int InputManager::GetInputCount() const
{
  return mHandler->GetInputCount();
}

/**
 * @brief Update loop
 */
void InputManager::Update()
{
  mHandler->Update();
  for(InputIT it = mInputs.begin(); it != mInputs.end();)
  {
    GetOwningApp()->SendMessageDelayed(new InputMessage((*it)->mInput, (*it)->mLocation, (*it)->mId));
    if((*it)->mSingleFrame)
    {
      delete *it;
      it = mInputs.erase(it);
    }
    else
      ++it;
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

/**
 * @brief Make this manager visible in LUA.
 */
void InputManager::SerializeLUA()
{
  SLB::Class<InputManager>("InputManager").inherits<Manager>()
    .set("GetInputCount", &InputManager::GetInputCount);
}