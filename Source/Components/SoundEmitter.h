#ifndef __JFramework_SoundEmitter_h_
#define __JFramework_SoundEmitter_h_

#include "Component.h" // Base class: Component
#include "Common.h"
#include "MathExt.h"
#include "SoundListener.h"

enum SoundEmitPattern
{
  CONSTANT,
  HORIZONTAL_LINE,
  VERTICAL_LINE,
  RADIAL
};

class SoundEmitter : public Component
{
private:
  SoundEmitPattern mPattern;
  HashString mSoundName;
  Vector3 mSoundOrigin;
  int mChannel;
  float mVolume;
  
  std::set<SoundListener*> mListeners;
  
  static int const sUID;
  
public:
  SoundEmitter();
  SoundEmitter(SoundEmitter const &aSoundEmitter);
  virtual ~SoundEmitter();
  
  // GETTERS
  SoundEmitPattern GetSoundEmitPattern() const;
  HashString GetSoundName() const;
  Vector3 GetSoundOrigin() const;
  int GetChannel() const;
  
  // SETTERS
  void SetSoundEmitPattern(SoundEmitPattern const &aPattern);
  void SetSoundName(HashString const &aSoundName);
  void SetSoundOrigin(Vector3 const &aSoundOrigin);
  
  // METHODS
  void StopSound();
  
  // VIRTUALS
  virtual void Update();
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void SendMessage(Message const& aMessage);
  virtual void Serialize(ParserNode* aNode);
  virtual void Deserialize(ParserNode* aNode);
  virtual Component* Clone(GameObject *aNewOwner) const;
  
  // STATICS
  static void SerializeLUA();
  static int GetUID() {return sUID;}
};

#endif
