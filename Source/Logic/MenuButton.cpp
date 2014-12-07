#include "MenuButton.h"
#include "ClickMessage.h"
#include "Transform.h"

MenuButton::MenuButton(std::string const &aFilename) : MenuImage(aFilename), mClickableArea()
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
  // Get image data
  MenuImage::ParseAdditionalData(aParser);
  
  // This should be in screen space
  if(aParser.Find("ClickableArea"))
  {
    Vector3 position, size;
    position.x = aParser.Find("ClickableArea", "PositionX")->GetValue().ToFloat();
    position.y = aParser.Find("ClickableArea", "PositionY")->GetValue().ToFloat();
    position.z = aParser.Find("ClickableArea", "PositionZ")->GetValue().ToFloat();
    size.x = aParser.Find("ClickableArea", "SizeX")->GetValue().ToFloat();
    size.y = aParser.Find("ClickableArea", "SizeY")->GetValue().ToFloat();
    size.z = aParser.Find("ClickableArea", "SizeZ")->GetValue().ToFloat();
    
    mClickableArea = Cube(position, size);
  }
  else
  {
    // If there is no area specified in the resource file, default to the Transform component.
    mClickableArea = Cube(GetObject()->GET<Transform>()->GetPosition(), GetObject()->GET<Transform>()->GetSize());
  }
}

void MenuButton::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() == "Click")
  {
    ClickMessage *clickMessage = (ClickMessage*)&aMessage;
    if(mClickableArea.Get2DCollision(clickMessage->GetLocation()))
    {
      OnDownClick();
    }
  }
  else if(aMessage.GetDescription() == "Release")
  {
    OnUpClick();
  }
}

void MenuButton::SendMessage(Message const& aMessage)
{
}

