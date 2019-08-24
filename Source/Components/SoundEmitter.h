#ifndef __JFramework_SoundEmitter_h_
#define __JFramework_SoundEmitter_h_

#include "Component.h" // Base class: Component
#include "Common.h"
#include "MathExt.h"
#include "SoundListener.h"

enum SoundEmitPattern
{
  UNIFORM,
  HORIZONTAL_LINE,
  VERTICAL_LINE,
  RADIAL
};

enum SoundEmitFunction
{
  CONSTANT,
  LINEAR,
  SQUARED,
  INVERSE_SQUARED
};

class SoundEmitter : public Component
{
private:
  SoundEmitPattern mPattern;
  SoundEmitFunction mFunction;
  HashString mSoundName;
  HashString mChannelGroup;
  Vector3 mSoundOrigin;
  int mChannel;
  float mVolume;
  bool mActive;
  
  std::set<SoundListener*> mListeners;
  
  static HashType const sUID;
  
public:
  SoundEmitter();
  SoundEmitter(SoundEmitter const &aSoundEmitter);
  virtual ~SoundEmitter();
  
  // GETTERS
  SoundEmitPattern GetSoundEmitPattern() const;
  SoundEmitFunction GetSoundEmitFunction() const;
  HashString GetSoundName() const;
  HashString GetChannelGroup() const;
  Vector3 GetSoundOrigin() const;
  int GetChannel() const;
  bool GetActive() const;
  
  // SETTERS
  void SetSoundEmitPattern(SoundEmitPattern const &aPattern);
  void SetSoundEmitFunction(SoundEmitFunction const &aFunction);
  void SetSoundName(HashString const &aSoundName);
  void SetChannelGroup(HashString const &aChannelGroup);
  void SetSoundOrigin(Vector3 const &aSoundOrigin);
  void SetActive(bool const &aActive);
  
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
  static HashType GetUID() {return sUID;}
};

#endif
