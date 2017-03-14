#ifndef __JFramework_DefaultChemicalFactory_h_
#define __JFramework_DefaultChemicalFactory_h_

#include "ChemicalFactory.h"

class DefaultChemicalFactory : public ChemicalFactory
{
public:
  DefaultChemicalFactory();
  virtual ~DefaultChemicalFactory();

  virtual ChemistryMaterial* CreateMaterial(ChemistryManager *aManager, HashString const &aName);
  virtual ChemistryElement* CreateElement(ChemistryManager *aManager, HashString const &aName);
};

#endif // __JFramework_DefaultChemicalFactory_h_
