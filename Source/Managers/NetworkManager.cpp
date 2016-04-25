#include "NetworkManager.h"
#include "NetworkMessage.h"

NetworkManager::NetworkManager() : mPorts()
{
}

NetworkManager::~NetworkManager()
{
}

void NetworkManager::SendNetworkMessage(HashString const &aIPAddress, HashString const &aMessage)
{
  std::map<HashString,NetworkPort*>::iterator port = mPorts.find(aIPAddress);
  
  // If no port, log and fail.
  if(port == mPorts.end())
  {
    DebugLogPrint("Port %s not found", aIPAddress.ToCharArray());
    return;
  }
  
  port->second->Send(aMessage);
}

HashString const NetworkManager::ReceiveNetworkMessage(HashString const &aIPAddress)
{
  std::map<HashString,NetworkPort*>::iterator port = mPorts.find(aIPAddress);
  
  // If no port, log and fail.
  if(port == mPorts.end())
  {
    DebugLogPrint("Port %s not found", aIPAddress.ToCharArray());
    return "";
  }
  
  return port->second->Receive();
}

void NetworkManager::Update()
{
  std::map<HashString,NetworkPort*>::iterator end = mPorts.end();
  for(std::map<HashString,NetworkPort*>::iterator it = mPorts.begin(); it != end; ++it)
  {
    HashString messageContent = it->second->Receive();
    
    if(messageContent.Empty())
    {
      continue;
    }
    
    NetworkMessage message(messageContent);
    GetOwningApp()->SendMessage(message);
  }
}

void NetworkManager::SendMessage(Message const& aMessage)
{
}

void NetworkManager::ProcessDelayedMessage(Message* aMessage)
{
  SendNetworkMessage(aMessage->GetDescription(), aMessage->GetContent());
}