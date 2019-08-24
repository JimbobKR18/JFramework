#ifndef __JFramework_StateObject_h_
#define __JFramework_StateObject_h_

#include "Component.h"
#include "StateMachine.h"

class StateObject : public Component
{
private:
  StateMachine *mStateMachine;
  
  static HashType const sUID;
  
public:
  StateObject();
  StateObject(StateObject const &aStateObject);
  virtual ~StateObject();
  
  StateMachine *GetStateMachine() const;

  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(ParserNode *aNode);
  virtual void Deserialize(ParserNode *aNode);
  virtual void Update();
  virtual Component* Clone(GameObject *aNewOwner) const;
  static void  SerializeLUA();
  static HashType GetUID() {return sUID;}
};

#endif // __JFramework_StateObject_h_
