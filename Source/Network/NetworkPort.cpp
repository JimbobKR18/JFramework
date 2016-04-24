#include "NetworkPort.h"

NetworkPort::NetworkPort() : mIPAddress()
{
}

NetworkPort::~NetworkPort()
{
}

HashString const& NetworkPort::GetIPAddress() const
{
  return mIPAddress;
}