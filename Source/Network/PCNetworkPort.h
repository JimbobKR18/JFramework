#ifndef __JFramework_PCNetworkPort_h_
#define __JFramework_PCNetworkPort_h_

#include "NetworkPort.h" // Base class: NetworkPort

#if defined(_WIN32)
  #include <SDL_net.h>
#else
  #include <SDL2/SDL_net.h>
#endif

class PCNetworkPort : public NetworkPort
{
private:
  UDPsocket mSocket;
  
public:
  PCNetworkPort();
  virtual ~PCNetworkPort();
  
  // Virtuals
  virtual void Open();
  virtual void Close();
  virtual void Bind(HashString const &aIPAddress);
  virtual void Unbind();
  virtual void Send(HashString const &aMessage);
  virtual HashString Receive();
  virtual void SendMany(std::vector<HashString> const &aMessages);
  virtual std::vector<HashString> ReceiveMany();
};

#endif
