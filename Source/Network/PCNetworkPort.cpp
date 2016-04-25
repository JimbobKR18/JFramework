#include "PCNetworkPort.h"
#include "Constants.h"

PCNetworkPort::PCNetworkPort() : mSocket()
{
}

PCNetworkPort::~PCNetworkPort()
{
}

/**
 * @brief Open socket
 */
void PCNetworkPort::Open()
{
  mSocket = SDLNet_UDP_Open(0);
}

/**
 * @brief Close socket
 */
void PCNetworkPort::Close()
{
  SDLNet_UDP_Close(mSocket);
  mSocket = NULL;
}

/**
 * @brief Bind socket via UDP
 * @param aIPAddress IP address to bind to
 */
void PCNetworkPort::Bind(HashString const &aIPAddress)
{
  IPaddress ip;
  std::vector<HashString> ipSplit = GetIPAddress().Split(":");
  ip.host = ipSplit[0].ToInt();
  ip.port = ipSplit[1].ToInt();
  SDLNet_UDP_Bind(mSocket, -1, &ip);
}

/**
 * @brief Unbind socket
 */
void PCNetworkPort::Unbind()
{
  SDLNet_UDP_Unbind(mSocket, 0);
}

/**
 * @brief Send message through port via UDP
 * @param aMessage Message to send
 */
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

/**
 * @brief Receive message through port via UDP
 * @return Message as string
 */
HashString PCNetworkPort::Receive()
{
  UDPpacket *packet = nullptr;
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

/**
 * @brief Send messages through port via UDP
 * @param aMessages Messages to send
 */
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

/**
 * @brief Receive messages through port via UDP
 * @return Messages as string vector
 */
std::vector<HashString> PCNetworkPort::ReceiveMany()
{
  std::vector<HashString> ret;
  UDPpacket **packetVector = nullptr;
  
  int numRecv = SDLNet_UDP_RecvV(mSocket, packetVector);
  if(numRecv == -1)
  {
    DebugLogPrint("Unable to receive packets because %s\n", SDLNet_GetError());
  }
  
  for(int i = 0; i < numRecv; ++i)
  {
    ret.push_back((char*)packetVector[i]->data);
  }
  
  SDLNet_FreePacketV(packetVector);
  return ret;
}