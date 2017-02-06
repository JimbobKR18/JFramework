#ifndef __JFramework_EffectManager_h_
#define __JFramework_EffectManager_h_

#include "Common.h"
#include "Effect.h"

class EffectsManager
{
public:
  typedef std::vector<Effect*>           EffectContainer;
  typedef std::vector<Effect*>::iterator EffectIT;
  
private:
  EffectContainer mEffects;
  
public:
  EffectsManager();
  ~EffectsManager();

  void AddEffect(Effect* aEffect);
  void RemoveEffect(HashString const &aName);
  void RemoveMatchingEffects(HashString const &aName);
  void RemoveEffectsForObject(GameObject *aObject);
  Effect *GetEffect(HashString const &aName);

  bool Empty();
  void RemoveEffects();

  void Update(float const aDT);
};

#endif
