#ifndef __JFramework_NetworkManager_h_
#define __JFramework_NetworkManager_h_

#include "Common.h"
#include "Manager.h" // Base class: Manager
#include "NetworkBase.h"

class NetworkManager : public Manager
{
private:
  NetworkBase *mBase;
  
public:
  NetworkManager();
  virtual ~NetworkManager();

  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ProcessDelayedMessage(Message* aMessage);
};

#endif
