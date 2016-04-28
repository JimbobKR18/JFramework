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
  
  std::vector<SDL_Joystick*>::iterator end = mJoysticks.end();
  
  // Removal
  for(std::vector<SDL_Joystick*>::iterator it = mJoysticks.begin(); it != end;)
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