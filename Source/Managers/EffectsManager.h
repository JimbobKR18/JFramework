#ifndef __JFramework_EffectsManager_h_
#define __JFramework_EffectsManager_h_

#include "Common.h"
#include "Manager.h"
#include "Effect.h"

class EffectsManager : public Manager
{
public:
  typedef std::vector<Effect*>            EffectContainer;
  typedef EffectContainer::iterator       EffectIT;
  typedef EffectContainer::const_iterator ConstEffectIT;
  
private:
  EffectContainer mEffects;
  
  static unsigned const sUID;
  
public:
  EffectsManager(GameApp *aApp);
  virtual ~EffectsManager();

  void AddEffect(Effect* aEffect);
  void RemoveEffect(HashString const &aName);
  void RemoveMatchingEffects(HashString const &aName);
  void RemoveEffectsForObject(GameObject *aObject);
  Effect *GetEffect(HashString const &aName);

  bool Empty();
  void RemoveEffects();

  virtual void Update();
  virtual void ProcessDelayedMessage(Message* aMessage);
  virtual void SendMessage(Message const& aMessage);
  
  static std::string  GetName() {return "EffectsManager";}
  static unsigned     GetUID() {return sUID;}
};

#endif
