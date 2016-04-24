#include "PCNetworkPort.h"

PCNetworkPort::PCNetworkPort() : mSocket()
{
}

PCNetworkPort::~PCNetworkPort()
{
}

void PCNetworkPort::Open()
{
  mSocket = SDLNet_UDP_Open(0);
}

void PCNetworkPort::Close()
{
  SDLNet_UDP_Close(mSocket);
  mSocket = NULL;
}

void PCNetworkPort::Bind(HashString const &aIPAddress)
{
  IPaddress ip;
  std::vector<HashString> ipSplit = GetIPAddress().Split(":");
  ip.host = ipSplit[0].ToInt();
  ip.port = ipSplit[1].ToInt();
  SDLNet_UDP_Bind(mSocket, -1, &ip);
}

void PCNetworkPort::Unbind()
{
  SDLNet_UDP_Unbind(mSocket, 0);
}

void PCNetworkPort::Send(HashString const &aMessage)
{
  UDPpacket packet;
}

HashString PCNetworkPort::Receive()
{
  
}

void PCNetworkPort::SendMany(std::vector<HashString> const &aMessages)
{
  
}

std::vector<HashString> PCNetworkPort::ReceiveMany()
{
  
}