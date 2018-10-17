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
    DeviceInfo *info = it->second;
    SDL_GameController* gameController = mGameControllers[info->GetId()];
    /*info->SetAxis(0, SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX));
    info->SetAxis(1, SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY));
    info->SetAxis(2, SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX));
    info->SetAxis(3, SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY));
    info->SetAxis(4, SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    info->SetAxis(5, SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));*/
  }
}

/**
 * @brief On device add detected
 * @param aId Id of input
 */
void PCInputHandler::DeviceAdd(int const aId)
{
  SDL_GameController* gameController = SDL_GameControllerOpen(aId);
  HashString stickName = SDL_GameControllerName(gameController);
  mDevices[aId] = new DeviceInfo(aId, stickName);
  mGameControllers[aId] = gameController;
}

/**
 * @brief On device remove detected
 * @param aId Id of input
 */
void PCInputHandler::DeviceRemove(int const aId)
{
  delete mDevices[aId];
  mDevices.erase(aId);
  SDL_GameControllerClose(mGameControllers[aId]);
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
  std::unordered_map<int, SDL_GameController*>::const_iterator instance = mGameControllers.find(index);
  if(instance == mGameControllers.end())
  {
    return nullptr;
  }
  
  return instance->second;
}
