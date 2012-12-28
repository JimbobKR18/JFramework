#include "Surface.h"

Surface::Surface() : Component("Surface")
{
}

void Surface::Update()
{
}

void Surface::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

void Surface::ReceiveMessage(Message const &aMessage)
{
}
