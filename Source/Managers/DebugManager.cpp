/*
 * DebugManager.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: jimbob
 */

#include "DebugManager.h"
#include "GraphicsManager.h"

unsigned const DebugManager::sUID = Common::StringHashFunction("DebugManager");
DebugManager::DebugManager(GameApp *aApp) : Manager(aApp, "DebugManager", DebugManager::sUID), mInvalidMemory()
{
}

DebugManager::~DebugManager()
{
}

int DebugManager::HandleEvent(SDL_Event &event)
{
  return 0;
}

void DebugManager::HandleCreate(ObjectCreateMessage *aMsg)
{
  size_t address = (size_t)aMsg->mObject;
  if(mInvalidMemory.find(address) != mInvalidMemory.end())
    mInvalidMemory.erase(mInvalidMemory.find(address));
}

void DebugManager::HandleDelete(ObjectDeleteMessage *aMsg)
{
  size_t address = (size_t)aMsg->mObject;
  if(mInvalidMemory.find(address) == mInvalidMemory.end())
    mInvalidMemory[address] = 0;
  int &numDeletes = mInvalidMemory[address];
  numDeletes += 1;
  if(numDeletes > 1)
    assert(!"Double delete occurred somewhere. Check this out.");
}

void DebugManager::Update()
{
  // Clear out, we only want to check for double deletes in a single frame.
  mInvalidMemory.clear();
}

void DebugManager::SendMessage(Message const &aMessage)
{
}

void DebugManager::ProcessDelayedMessage(Message *aMessage)
{
}
