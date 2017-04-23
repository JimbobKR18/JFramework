#ifndef __JFramework_DefaultEffectsFactory_h_
#define __JFramework_DefaultEffectsFactory_h_

#include "EffectsFactory.h"

class DefaultEffectsFactory : public EffectsFactory
{
public:
  DefaultEffectsFactory();
  virtual ~DefaultEffectsFactory();
  
  virtual Effect* CreateEffect(EffectsManager* aManager, HashString const& aType);
};

#endif // __JFramework_DefaultEffectsFactory_h_
