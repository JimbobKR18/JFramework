#include "PCInputHandler.h"

PCInputHandler::PCInputHandler() : InputHandler(), mJoysticks()
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
  for(std::unordered_map<int, DeviceInfo*>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
  {
    DeviceInfo *info = it->second;
    SDL_Joystick* joystick = mJoysticks[info->GetId()];
    info->SetAxis(0, SDL_JoystickGetAxis(joystick, 0));
    info->SetAxis(1, SDL_JoystickGetAxis(joystick, 1));
    info->SetAxis(2, SDL_JoystickGetAxis(joystick, 2));
    info->SetAxis(3, SDL_JoystickGetAxis(joystick, 3));
    info->SetAxis(4, SDL_JoystickGetAxis(joystick, 4));
    info->SetAxis(5, SDL_JoystickGetAxis(joystick, 5));
  }
}

/**
 * @brief On device add detected
 * @param aId Id of input
 */
void PCInputHandler::DeviceAdd(int const aId)
{
  SDL_Joystick* joystick = SDL_JoystickOpen(aId);
  int instanceId = SDL_JoystickInstanceID(joystick);
  HashString stickName = SDL_JoystickName(joystick);
  mDevices[instanceId] = new DeviceInfo(instanceId, stickName);
  mJoysticks[instanceId] = joystick;
}

/**
 * @brief On device remove detected
 * @param aId Id of input
 */
void PCInputHandler::DeviceRemove(int const aId)
{
  int instanceId = SDL_JoystickInstanceID(mJoysticks[aId]);
  delete mDevices[instanceId];
  mDevices.erase(instanceId);
  SDL_JoystickClose(mJoysticks[instanceId]);
  mJoysticks.erase(instanceId);
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
  std::unordered_map<int, SDL_Joystick*>::const_iterator instance = mJoysticks.find(index);
  if(instance == mJoysticks.end())
  {
    return nullptr;
  }
  
  return instance->second;
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
