#include "MenuButton.h"

MenuButton::MenuButton(std::string const &aFilename) : MenuElement(aFilename)
{
}

MenuButton::~MenuButton()
{
}

void MenuButton::Draw()
{
  
}

void MenuButton::ParseAdditionalData(Parser& aParser)
{
  
}

void MenuButton::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() == "Click")
  {
    OnDownClick();
  }
  else if(aMessage.GetDescription() == "Release")
  {
    OnUpClick();
  }
}

void MenuButton::SendMessage(Message const& aMessage)
{
  
}

