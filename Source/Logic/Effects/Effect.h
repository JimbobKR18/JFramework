#ifndef __JFramework_Effect_h_
#define __JFramework_Effect_h_

#include "Common.h"
#include "GameObject.h"

class Effect
{
private:
  GameObject* mObject;
  float       mTime;
  HashString  mName;

public:
  Effect(HashString const &aName, GameObject *aObject, float aTime);
  virtual ~Effect();

  // GETTERS
  GameObject* GetObject() const;
  float const GetTime() const;
  HashString const GetName() const;
  
  // SETTERS
  void SetObject(GameObject* aObject);
  void SetTime(float aTime);
  void SetName(HashString const &aName);

  // VIRTUALS
  virtual void Update(float const aDT) = 0;
  virtual void Complete() = 0;
  virtual bool IsComplete() = 0;
  virtual void Deserialize(Root* aRoot) = 0;
};

#endif
