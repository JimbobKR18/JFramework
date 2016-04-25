#include "NetworkManager.h"
#include "NetworkMessage.h"

NetworkManager::NetworkManager() : mPorts()
{
}

NetworkManager::~NetworkManager()
{
}

/**
 * @brief Send message across network.
 * @param aIPAddress IP address to send to.
 * @param aMessage Message to send.
 */
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

/**
 * @brief Receive message from IP address
 * @param aIPAddress IP address to receive from
 * @return Message as string
 */
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

/**
 * @brief Update loop
 */
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

/**
 * @brief Does nothing
 * @param aMessage Message to do nothing with
 */
void NetworkManager::SendMessage(Message const& aMessage)
{
}

/**
 * @brief Send message across network
 * @param aMessage Message to send
 */
void NetworkManager::ProcessDelayedMessage(Message* aMessage)
{
  SendNetworkMessage(aMessage->GetDescription(), aMessage->GetContent());
}