#include "Button.h"
#include "ClickMessage.h"
#include "Transform.h"
#include "LUA/LUATypes.h"

int const Button::sUID = Common::StringHashFunction("Button");

Button::Button() : Component(Button::sUID), mClickableArea()
{
}

Button::~Button()
{
}

/**
 * @brief Does nothing
 */
void Button::Update()
{
}

/**
 * @brief Does nothing (yet)
 * @param aMessage
 */
void Button::SendMessage(Message const& aMessage)
{
}

/**
 * @brief Receive click messages.
 * @param aMessage
 */
void Button::ReceiveMessage(Message const& aMessage)
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
 * @brief Gather data from deserialized file.
 * @param aNode
 */
void Button::Deserialize(ParserNode *aNode)
{
  // This should be in screen space
  if(aNode->Find("ClickableArea"))
  {
    Vector3 position, size;
    position.x = aNode->Find("ClickableArea")->Find("PositionX")->GetValue().ToFloat();
    position.y = aNode->Find("ClickableArea")->Find("PositionY")->GetValue().ToFloat();
    position.z = aNode->Find("ClickableArea")->Find("PositionZ")->GetValue().ToFloat();
    size.x = aNode->Find("ClickableArea")->Find("SizeX")->GetValue().ToFloat();
    size.y = aNode->Find("ClickableArea")->Find("SizeY")->GetValue().ToFloat();
    size.z = aNode->Find("ClickableArea")->Find("SizeZ")->GetValue().ToFloat();
    
    mClickableArea = AxisAlignedBoundingBox(position, size);
  }
  else
  {
    // If there is no area specified in the resource file, default to the Transform component.
    mClickableArea = AxisAlignedBoundingBox(GetOwner()->GET<Transform>()->GetPosition(), GetOwner()->GET<Transform>()->GetSize());
  }
}

/**
 * @brief Serialize to file (cannot handle rotations)
 * @param aNode ParserNode to serialize to.
 */
void Button::Serialize(ParserNode *aNode)
{
  HashString const BUTTON = "Button";
  aNode->Place(BUTTON, "");
  ParserNode* button = aNode->Find(BUTTON);

  button->Place("ClickableArea", "");
  ParserNode *clickableArea = button->Find("ClickableArea");
  
  clickableArea->Place("PositionX", Common::FloatToString(mClickableArea.position.x));
  clickableArea->Place("PositionY", Common::FloatToString(mClickableArea.position.y));
  clickableArea->Place("PositionZ", Common::FloatToString(mClickableArea.position.z));
  clickableArea->Place("SizeX", Common::FloatToString(mClickableArea.size.x));
  clickableArea->Place("SizeY", Common::FloatToString(mClickableArea.size.y));
  clickableArea->Place("SizeZ", Common::FloatToString(mClickableArea.size.z));
}
