#ifndef __JFramework_SoundListener_h_
#define __JFramework_SoundListener_h_

#include "Component.h" // Base class: Component
#include "Common.h"

class SoundListener : public Component
{
private:
  bool mActive;
  static HashType const sUID;
  
public:
  SoundListener();
  SoundListener(SoundListener const &aSoundListener);
  virtual ~SoundListener();
  
  bool GetActive() const;
  void SetActive(bool const &aActive);

  // VIRTUALS
  virtual void Update();
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void SendMessage(Message const& aMessage);
  virtual void Serialize(ParserNode* aNode);
  virtual void Deserialize(ParserNode* aNode);
  virtual Component* Clone(GameObject *aNewOwner) const;
  
  // STATICS
  static void SerializeLUA();
  static HashType GetUID() {return sUID;}
};

#endif
