/*
 * DebugManager.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: jimbob
 */

#include "DebugManager.h"
#include "TextParser.h"
#include "ParserFactory.h"

HashType const DebugManager::sUID = Common::StringHashFunction("DebugManager");
DebugManager::DebugManager(GameApp *aApp) : Manager(aApp, "DebugManager", DebugManager::sUID), mInvalidMemory()
{
}

DebugManager::~DebugManager()
{
}

/**
 * @brief Handle SDL Event
 * @param event Event to handle
 * @return 0
 */
int DebugManager::HandleEvent(SDL_Event &event)
{
  return 0;
}

/**
 * @brief Handle object creation
 * @param aMsg Create message
 */
void DebugManager::HandleCreate(ObjectCreateMessage *aMsg)
{
  size_t address = (size_t)aMsg->mObject;
  if(mInvalidMemory.find(address) != mInvalidMemory.end())
    mInvalidMemory.erase(mInvalidMemory.find(address));
}

/**
 * @brief Handle object deletion
 * @param aMsg Delete message
 */
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

/**
 * @brief Log exception
 * @param aException Exception to log
 */
void DebugManager::LogException(std::exception const &aException)
{
  char file[256];
  sprintf(file, "CrashLog_%d.txt", time(0));
  Parser *parser = ParserFactory::CreateOutputParser("", file);
  ParserNode *root = parser->GetBaseRoot();
  root->Place("Message", aException.what());
  parser->Write();
  delete parser;
  
  DebugLogPrint(aException.what());
}

/**
 * @brief Update loop
 */
void DebugManager::Update()
{
  // Clear out, we only want to check for double deletes in a single frame.
  mInvalidMemory.clear();
}

/**
 * @brief Send message
 * @param aMessage Message to send
 */
void DebugManager::SendMessage(Message const &aMessage)
{
}

/**
 * @brief Process delayed message
 * @param aMessage Delayed message
 */
void DebugManager::ProcessDelayedMessage(Message *aMessage)
{
}
