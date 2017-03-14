#include "DefaultChemicalFactory.h"
#include "ChemistryManager.h"

DefaultChemicalFactory::DefaultChemicalFactory() : ChemicalFactory()
{
}

DefaultChemicalFactory::~DefaultChemicalFactory()
{
}

ChemistryMaterial* DefaultChemicalFactory::CreateMaterial(ChemistryManager *aManager, HashString const &aName)
{
  ChemistryMaterial *material = new ChemistryMaterial(aManager);
  return material;
}

ChemistryElement* DefaultChemicalFactory::CreateElement(ChemistryManager *aManager, HashString const &aName)
{
  ChemistryElement *element = new ChemistryElement(aManager);
  return element;
}
