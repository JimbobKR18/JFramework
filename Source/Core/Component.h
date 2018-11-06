#ifndef __JFramework__Component__
#define __JFramework__Component__

#include "GameObject.h"
#include "Message.h"
#include "Parser.h"
#include <assert.h>

#define DEFAULT_COMPONENT_UID 0

class Component
{
private:
  GameObject* mOwner;
  int mUID;

public:
  Component();
  Component(int const &aUID);
  Component(Component const &aComponent);
  virtual ~Component();

  GameObject*         GetOwner() const;
  void                SetOwner(GameObject *aOwner);
  int                 GetDefinedUID() const;

  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(ParserNode *aNode) = 0;
  virtual void        Deserialize(ParserNode *aNode) = 0;
  virtual Component*  Clone(GameObject *aNewOwner) const = 0;
  static int          GetUID() {return DEFAULT_COMPONENT_UID;}
  static void         SerializeLUA();
};

#endif /* defined(__JFramework__Component__) */
