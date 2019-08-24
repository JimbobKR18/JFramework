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
  bool mInitialized;
  HashType mUID;

public:
  Component();
  Component(HashType const &aUID);
  Component(Component const &aComponent);
  virtual ~Component();

  GameObject*         GetOwner() const;
  void                SetOwner(GameObject *aOwner);
  HashType            GetDefinedUID() const;
  bool                GetInitialized() const;

  virtual void        Initialize();
  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(ParserNode *aNode) = 0;
  virtual void        Deserialize(ParserNode *aNode) = 0;
  virtual Component*  Clone(GameObject *aNewOwner) const = 0;
  static HashType GetUID() {return DEFAULT_COMPONENT_UID;}
  static void         SerializeLUA();
};

#endif /* defined(__JFramework__Component__) */
