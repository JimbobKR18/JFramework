#include "PCNetworkPort.h"
#include "Constants.h"

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
  UDPpacket *packet = SDLNet_AllocPacket(Constants::GetInteger("PacketSize"));
  packet->len = aMessage.Length();
  packet->data = (Uint8*)aMessage.ToCharArray();
  if(!SDLNet_UDP_Send(mSocket, 0, packet))
  {
    DebugLogPrint("Unable to send packet because %s\n", SDLNet_GetError());
  }
  SDLNet_FreePacket(packet);
}

HashString PCNetworkPort::Receive()
{
  UDPpacket *packet = SDLNet_AllocPacket(Constants::GetInteger("PacketSize"));
  HashString ret;
  if(SDLNet_UDP_Recv(mSocket, packet) == -1)
  {
    DebugLogPrint("Unable to receive packet because %s\n", SDLNet_GetError());
  }
  else
  {
    ret = (char*)packet->data;
  }
  SDLNet_FreePacket(packet);
  return ret;
}

void PCNetworkPort::SendMany(std::vector<HashString> const &aMessages)
{
  UDPpacket **packetVector = SDLNet_AllocPacketV(aMessages.size(), Constants::GetInteger("PacketSize"));
  
  for(int i = 0; i < aMessages.size(); ++i)
  {
    packetVector[i]->len = aMessages[i].Length();
    packetVector[i]->data = (Uint8*)aMessages[i].ToCharArray();
  }
  
  if(!SDLNet_UDP_SendV(mSocket, packetVector, aMessages.size()))
  {
    DebugLogPrint("Unable to send packets because %s\n", SDLNet_GetError());
  }
  
  SDLNet_FreePacketV(packetVector);
}

std::vector<HashString> PCNetworkPort::ReceiveMany()
{
  // TODO
  /*std::vector<HashString> ret;
  UDPpacket **packetVector = SDLNet_AllocPacketV(aMessages.size(), Constants::GetInteger("PacketSize"));
  
  if(!SDLNet_UDP_SendV(mSocket, packetVector, aMessages.size()))
  {
    DebugLogPrint("Unable to send packets because %s\n", SDLNet_GetError());
  }
  
  SDLNet_FreePacketV(packetVector);*/
}