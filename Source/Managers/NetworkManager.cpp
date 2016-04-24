#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

void NetworkManager::Update()
{
}

void NetworkManager::SendMessage(Message const& aMessage)
{
  if(!aMessage.GetDescription().Find("Network"))
  {
    GetOwningApp()->SendMessage(aMessage);
  }
}

void NetworkManager::ProcessDelayedMessage(Message* aMessage)
{
}