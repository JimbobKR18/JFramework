#include "PCInputHandler.h"

PCInputHandler::PCInputHandler() : mJoysticks()
{
  SDL_JoystickEventState(SDL_ENABLE);
}

PCInputHandler::~PCInputHandler()
{
}

/**
 * @brief Keeps track of joysticks and removes / adds joysticks as necessary.
 */
void PCInputHandler::Update()
{
  unsigned numJoysticks = SDL_NumJoysticks();
  
  // Removal
  for(std::vector<SDL_Joystick*>::iterator it = mJoysticks.begin(); it != mJoysticks.end();)
  {
    char const* joystickName = SDL_JoystickName(*it);
    
    if(joystickName == nullptr)
    {
      it = mJoysticks.erase(it);
    }
    else
    {
      ++it;
    }
  }
  
  // Addition
  if(numJoysticks > mJoysticks.size())
  {
    for(unsigned i = 0; i < numJoysticks; ++i)
      mJoysticks.push_back(SDL_JoystickOpen(i));
  }
}

/**
 * @brief Get total number of input devices.
 * @return Number of input devices.
 */
int PCInputHandler::GetInputCount()
{
  return mJoysticks.size();
}

/**
 * @brief Get a particular joystick.
 * @param index Index of joystick.
 * @return nullptr if out of range. Otherwise, joystick at index.
 */
SDL_Joystick* PCInputHandler::GetJoystick(int const index) const
{
  if(index < 0 || index >= mJoysticks.size())
  {
    return nullptr;
  }
  
  return mJoysticks[index];
}

/**
 * @brief Get instance id of joystick.
 * @param index Index of joystick in mapping.
 * @return Actual joystick instance id.
 */
int PCInputHandler::GetJoystickInstanceId(int const index) const
{
  SDL_Joystick* joystick = GetJoystick(index);
  return SDL_JoystickInstanceID(joystick);
}