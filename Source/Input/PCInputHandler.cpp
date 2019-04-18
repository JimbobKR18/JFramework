#include "PCInputHandler.h"

PCInputHandler::PCInputHandler() : InputHandler(), mGameControllers()
{
  SDL_GameControllerEventState(SDL_ENABLE);
  SDL_GameControllerAddMappingsFromFile(Common::RelativePath("Input", "gamecontrollerdb.txt").c_str());
}

PCInputHandler::~PCInputHandler()
{
}

/**
 * @brief Keeps track of GameControllers and removes / adds GameControllers as necessary.
 */
void PCInputHandler::Update()
{
  for(std::unordered_map<int, DeviceInfo*>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
  {
  }
}

/**
 * @brief On device add detected
 * @param aId Id of input
 */
void PCInputHandler::DeviceAdd(int const aId)
{
  JoypadController *joyPad = new JoypadController();
  SDL_GameController* gameController = SDL_GameControllerOpen(aId);
  HashString stickName = SDL_GameControllerName(gameController);
  SDL_Joystick *j = SDL_GameControllerGetJoystick(gameController);
  
  joyPad->mController = gameController;
  joyPad->mInstanceId = SDL_JoystickInstanceID(j);
  
  // Get next available id.
  int nextId = 0;
  std::set<int> orderedControllerIds;
  for(std::unordered_map<int, DeviceInfo*>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
  {
    orderedControllerIds.insert(it->second->GetId());
  }
  while(orderedControllerIds.find(nextId) != orderedControllerIds.end())
    ++nextId;
  
  mDevices[joyPad->mInstanceId] = new DeviceInfo(nextId, stickName);
  mGameControllers[joyPad->mInstanceId] = joyPad;
}

/**
 * @brief On device remove detected
 * @param aId Id of input
 */
void PCInputHandler::DeviceRemove(int const aId)
{
  delete mDevices[aId];
  mDevices.erase(aId);
  SDL_GameControllerClose(mGameControllers[aId]->mController);
  delete mGameControllers[aId];
  mGameControllers.erase(aId);
}

/**
 * @brief Get total number of input devices.
 * @return Number of input devices.
 */
int PCInputHandler::GetInputCount()
{
  return mGameControllers.size();
}

/**
 * @brief Get a particular GameController.
 * @param index Index of GameController.
 * @return nullptr if out of range. Otherwise, GameController at index.
 */
SDL_GameController* PCInputHandler::GetGameController(int const index) const
{
  std::unordered_map<int, JoypadController*>::const_iterator instance = mGameControllers.find(index);
  if(instance == mGameControllers.end())
  {
    return nullptr;
  }
  
  return instance->second->mController;
}
