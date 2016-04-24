#include "NetworkBase.h"

NetworkBase::NetworkBase() : mPorts()
{
}

NetworkBase::~NetworkBase()
{
}

void NetworkBase::Update()
{
  std::map<HashString,NetworkPort*>::iterator end = mPorts.end();
  for(std::map<HashString,NetworkPort*>::iterator it = mPorts.begin(); it != end; ++it)
  {
    
  }
}

void NetworkBase::SendNetworkMessage(HashString const &aIPAddress, HashString const &aMessage)
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

HashString const NetworkBase::ReceiveNetworkMessage(HashString const &aIPAddress)
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