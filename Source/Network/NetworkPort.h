#ifndef __JFramework_NetworkPort_h_
#define __JFramework_NetworkPort_h_

#include "Common.h"

class NetworkPort
{
private:
  HashString mIPAddress;
  
public:
  NetworkPort();
  virtual ~NetworkPort();
  
  // Getters
  HashString const& GetIPAddress() const;
  
  // Virtuals
  virtual void Open() = 0;
  virtual void Close() = 0;
  virtual void Bind(HashString const &aIPAddress) = 0;
  virtual void Unbind() = 0;
  virtual void Send(HashString const &aMessage) = 0;
  virtual HashString Receive() = 0;
  virtual void SendMany(std::vector<HashString> const &aMessages) = 0;
  virtual std::vector<HashString> ReceiveMany() = 0;
};

#endif
