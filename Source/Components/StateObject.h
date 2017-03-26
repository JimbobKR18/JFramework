#ifndef __JFramework_StateObject_h_
#define __JFramework_StateObject_h_

#include "Component.h"
#include "StateMachine.h"

class StateObject : public Component
{
private:
  StateMachine *mStateMachine;
  
  static int const sUID;
  
public:
  StateObject();
  virtual ~StateObject();
  
  StateMachine *GetStateMachine() const;

  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(Parser& aParser);
  virtual void Deserialize(Parser& aParser);
  virtual void Update();
  static void  SerializeLUA();
  static int   GetUID() {return sUID;}
};

#endif // __JFramework_StateObject_h_
