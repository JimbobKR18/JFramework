/*
 * DebugManager.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: jimbob
 */

#include "DebugManager.h"
#include "GraphicsManager.h"

DebugManager::DebugManager(GameApp *aApp) : Manager(aApp, "DebugManager")
{
}

DebugManager::~DebugManager()
{
}

int DebugManager::HandleEvent(SDL_Event &event)
{
  return 0;
}

void DebugManager::Update()
{
}

void DebugManager::SendMessage(Message const &aMessage)
{
}

void DebugManager::ProcessDelayedMessage(Message *aMessage)
{
}
