#ifndef __JFramework_ChemicalFactory_h_
#define __JFramework_ChemicalFactory_h_

#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "ChemistryManager.h"

class ChemicalFactory
{
public:
  ChemicalFactory() {}
  virtual ~ChemicalFactory() {}
  
  virtual ChemistryMaterial* CreateMaterial(ChemistryManager *aManager, HashString const &aName) = 0;
  virtual ChemistryElement* CreateElement(ChemistryManager *aManager, HashString const &aName) = 0;
};

#endif // __JFramework_ChemicalFactory_h_
