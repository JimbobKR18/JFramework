#include "NetworkPort.h"

NetworkPort::NetworkPort() : mIPAddress()
{
}

NetworkPort::~NetworkPort()
{
}

/**
 * @brief Get IP address of port
 * @return IP address
 */
HashString const& NetworkPort::GetIPAddress() const
{
  return mIPAddress;
}