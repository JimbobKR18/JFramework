#ifndef __JFramework_Button_h_
#define __JFramework_Button_h_

#include "Common.h"
#include "Component.h"
#include "Shape.h"

class Button : public Component
{
private:
  AxisAlignedBoundingBox mClickableArea;
  static int const sUID;
  
public:
  Button();
  virtual ~Button();

  // Derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(ParserNode *aNode);
  virtual void        Deserialize(ParserNode *aNode);
  virtual Component*  Clone(GameObject *aNewOwner) const = 0;
  virtual void        OnDownClick() = 0;
  virtual void        OnUpClick() = 0;
  static int          GetUID() {return sUID;}
};

#endif // __JFramework_Button_h_
