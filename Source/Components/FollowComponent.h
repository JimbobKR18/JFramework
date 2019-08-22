#ifndef __JFramework_FollowComponent_h_
#define __JFramework_FollowComponent_h_

#include "Common.h"
#include "Component.h"
#include "GameObject.h"
#include "Interpolation.h"
#include "MathExt.h"

class FollowComponent : public Component
{
private:
  GameObject *mTarget;
  HashString mTargetName;
  float mTime;
  Interpolation<Vector3> *mInterpolator;
  Vector3 mPosition;
  
  static int const sUID;
public:
  FollowComponent();
  FollowComponent(FollowComponent const &aFollowComponent);
  virtual ~FollowComponent();
  
  // Methods
  GameObject *GetTarget();
  void SetTarget(GameObject *aTarget);
  HashString GetTargetName() const;
  void SetTargetName(HashString const &aTargetName);
  void ResetTarget();
  float GetTime() const;
  void SetTime(float aTime);
  
  // Virtuals derived from Component
  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(ParserNode *aNode);
  virtual void Deserialize(ParserNode *aNode);
  virtual Component* Clone(GameObject *aNewOwner) const;
  
  // Statics
  static void SerializeLUA();
  static int GetUID() {return sUID;}
  
private:
  void ClearInterpolator();
};

#endif // __JFramework_FollowComponent_h_
