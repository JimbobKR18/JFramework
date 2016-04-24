#ifndef __JFramework_NetworkBase_h_
#define __JFramework_NetworkBase_h_

#include "Common.h"
#include "NetworkPort.h"

class NetworkBase
{
private:
  std::map<HashString,NetworkPort*> mPorts;
  
public:
  NetworkBase();
  virtual ~NetworkBase();
  
  void Update();
  void SendNetworkMessage(HashString const &aIPAddress, HashString const &aMessage);
  HashString const ReceiveNetworkMessage(HashString const &aIPAddress);
};

#endif
