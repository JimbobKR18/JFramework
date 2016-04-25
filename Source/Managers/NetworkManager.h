#ifndef __JFramework_NetworkManager_h_
#define __JFramework_NetworkManager_h_

#include "Common.h"
#include "Manager.h" // Base class: Manager
#include "NetworkPort.h"

class NetworkManager : public Manager
{
private:
  std::map<HashString,NetworkPort*> mPorts;
  
public:
  NetworkManager();
  virtual ~NetworkManager();
  
  void SendNetworkMessage(HashString const &aIPAddress, HashString const &aMessage);
  HashString const ReceiveNetworkMessage(HashString const &aIPAddress);

  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ProcessDelayedMessage(Message* aMessage);
};

#endif
