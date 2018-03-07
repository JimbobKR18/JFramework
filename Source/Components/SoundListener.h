#ifndef __JFramework_SoundListener_h_
#define __JFramework_SoundListener_h_

#include "Component.h" // Base class: Component
#include "Common.h"

class SoundListener : public Component
{
private:
  static int const sUID;
  
public:
  SoundListener();
  virtual ~SoundListener();

  virtual void Update();
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void SendMessage(Message const& aMessage);
  virtual void Serialize(ParserNode* aNode);
  virtual void Deserialize(ParserNode* aNode);
  
  static int GetUID() {return sUID;}
};

#endif
