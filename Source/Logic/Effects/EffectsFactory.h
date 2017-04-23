#ifndef __JFramework_EffectsFactory_h_
#define __JFramework_EffectsFactory_h_

#include "Effect.h"

class EffectsManager;
class EffectsFactory
{
public:
  EffectsFactory() {}
  virtual ~EffectsFactory() {}
  
  virtual Effect* CreateEffect(EffectsManager *aManager, HashString const &aType) = 0;
};

#endif // __JFramework_EffectsFactory_h_