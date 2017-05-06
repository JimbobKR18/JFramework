#include "MenuButton.h"
#include "ClickMessage.h"
#include "Transform.h"

MenuButton::MenuButton(Menu *aOwner, HashString const &aFilename) : MenuImage(aOwner, aFilename, false), mClickableArea()
{
  // MenuImage base hasn't initialized yet.
  TextParser parser(Common::RelativePath("Menus", aFilename));
  ParseAdditionalData(parser);
}

MenuButton::~MenuButton()
{
}

/**
 * @brief Does nothing
 */
void MenuButton::Draw()
{
}

/**
 * @brief Does nothing
 */
void MenuButton::Update()
{
}

/**
 * @brief Gather additional data from deserialized file.
 * @param aParser
 */
void MenuButton::ParseAdditionalData(Parser& aParser)
{
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
    
    mClickableArea = AxisAlignedBoundingBox(position, size);
  }
  else
  {
    // If there is no area specified in the resource file, default to the Transform component.
    mClickableArea = AxisAlignedBoundingBox(GetObject()->GET<Transform>()->GetPosition(), GetObject()->GET<Transform>()->GetSize());
  }
}

/**
 * @brief Receive click messages.
 * @param aMessage
 */
void MenuButton::ReceiveMessage(Message const& aMessage)
{
  if(aMessage.GetDescription() == "Click")
  {
    ClickMessage *clickMessage = (ClickMessage*)&aMessage;
    if(mClickableArea.Get2DCollision(clickMessage->GetLocation()))
    {
      clickMessage->SetContacted(true);
      OnDownClick();
    }
  }
  else if(aMessage.GetDescription() == "Release")
  {
    OnUpClick();
  }
}

/**
 * @brief Does nothing (yet)
 * @param aMessage
 */
void MenuButton::SendMessage(Message const& aMessage)
{
}

