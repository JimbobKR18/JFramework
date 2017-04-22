#include "DefaultChemicalFactory.h"
#include "ChemistryManager.h"

DefaultChemicalFactory::DefaultChemicalFactory() : ChemicalFactory()
{
}

DefaultChemicalFactory::~DefaultChemicalFactory()
{
}

/**
 * @brief Create new material.
 * @param aManager Manager associated with creation.
 * @param aName Name of material.
 * @return New material.
 */
ChemistryMaterial* DefaultChemicalFactory::CreateMaterial(ChemistryManager *aManager, HashString const &aName)
{
  ChemistryMaterial *material = new ChemistryMaterial(aManager);
  return material;
}

/**
 * @brief Create new element.
 * @param aManager Manager associated with creation.
 * @param aName Name of element.
 * @return New element.
 */
ChemistryElement* DefaultChemicalFactory::CreateElement(ChemistryManager *aManager, HashString const &aName)
{
  ChemistryElement *element = new ChemistryElement(aManager);
  return element;
}
