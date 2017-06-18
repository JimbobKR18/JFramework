#include "ChemistryManager.h"
#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "DefaultChemicalFactory.h"

unsigned const ChemistryManager::sUID = Common::StringHashFunction("ChemistryManager");

ChemistryManager::ChemistryManager(GameApp *aApp, float const aCurrentTemperature) : Manager(aApp, "ChemistryManager", ChemistryManager::sUID),
  mFactory(new DefaultChemicalFactory()), mMaterials(), mElements(), mCurrentTemperature(aCurrentTemperature)
{
}

ChemistryManager::~ChemistryManager()
{
  delete mFactory;
}

/**
 * @brief Set new chemical factory, delete old one.
 * @param aFactory Factory to set.
 */
void ChemistryManager::SetChemicalFactory(ChemicalFactory *aFactory)
{
  if(mFactory)
    delete mFactory;
  mFactory = aFactory;
}

/**
 * @brief Create new material.
 * @param aName Name of material type.
 * @return New material.
 */
ChemistryMaterial* ChemistryManager::CreateMaterial(HashString const &aName)
{
  ChemistryMaterial* material = mFactory->CreateMaterial(this, aName);
  AddMaterial(material);
  return material;
}

/**
 * @brief Delete and remove material.
 * @param aMaterial Material.
 */
void ChemistryManager::DeleteMaterial(ChemistryMaterial *aMaterial)
{
  RemoveMaterial(aMaterial);
  delete aMaterial;
}

/**
 * @brief Add material.
 * @param aMaterial Material to add.
 */
void ChemistryManager::AddMaterial(ChemistryMaterial *aMaterial)
{
  // Check to see if object is in our list
  MaterialIT materialEnd = mMaterials.end();
  for(MaterialIT it = mMaterials.begin(); it != materialEnd; ++it)
  {
    if(*it == aMaterial)
    {
      return;
    }
  }
  
  mMaterials.push_back(aMaterial);
}

/**
 * @brief Remove material.
 * @param aMaterial Material to remove.
 */
void ChemistryManager::RemoveMaterial(ChemistryMaterial *aMaterial)
{
  MaterialIT materialEnd = mMaterials.end();
  for(MaterialIT it = mMaterials.begin(); it != materialEnd; ++it)
  {
    if(*it == aMaterial)
    {
      mMaterials.erase(it);
      break;
    }
  }
}

/**
 * @brief Clear all materials.
 */
void ChemistryManager::ClearMaterials()
{
  MaterialIT materialEnd = mMaterials.end();
  for(MaterialIT it = mMaterials.begin(); it != materialEnd; ++it)
  {
    delete *it;
  }
  mMaterials.clear();
}
  
/**
 * @brief Create new element.
 * @param aName Name of element type.
 * @return New element.
 */
ChemistryElement* ChemistryManager::CreateElement(HashString const &aName)
{
  ChemistryElement* element = mFactory->CreateElement(this, aName);
  AddElement(element);
  return element;
}

/**
 * @brief Delete and remove element.
 * @param aElement Element.
 */
void ChemistryManager::DeleteElement(ChemistryElement *aElement)
{
  RemoveElement(aElement);
  delete aElement;
}

/**
 * @brief Add element.
 * @param aElement Element to add.
 */
void ChemistryManager::AddElement(ChemistryElement *aElement)
{
  // Check to see if object is in our list
  ElementIT elementEnd = mElements.end();
  for(ElementIT it = mElements.begin(); it != elementEnd; ++it)
  {
    if(*it == aElement)
    {
      return;
    }
  }
  
  mElements.push_back(aElement);
}

/**
 * @brief Remove element.
 * @param aElement Element to remove.
 */
void ChemistryManager::RemoveElement(ChemistryElement *aElement)
{
  ElementIT elementEnd = mElements.end();
  for(ElementIT it = mElements.begin(); it != elementEnd; ++it)
  {
    if(*it == aElement)
    {
      mElements.erase(it);
      break;
    }
  }
}

/**
 * @brief Clear all elements.
 */
void ChemistryManager::ClearElements()
{
  ElementIT elementEnd = mElements.end();
  for(ElementIT it = mElements.begin(); it != elementEnd; ++it)
  {
    delete *it;
  }
  mElements.clear();
}

/**
 * @brief Update loop.
 */
void ChemistryManager::Update()
{
  MaterialIT materialEnd = mMaterials.end();
  for(MaterialIT it = mMaterials.begin(); it != materialEnd; ++it)
  {
    float temperature = (*it)->GetCurrentTemperature();
    if(mCurrentTemperature > temperature)
    {
      temperature += (*it)->GetHeatTransferRate() * GetOwningApp()->GetAppStep();
    }
    else if(mCurrentTemperature < temperature)
    {
      temperature -= (*it)->GetHeatTransferRate() * GetOwningApp()->GetAppStep();
    }
    (*it)->SetCurrentTemperature(temperature);
  }
}

/**
 * @brief Does nothing.
 * @param aMessage Message to do nothing with.
 */
void ChemistryManager::ProcessDelayedMessage(Message* aMessage)
{
}

/**
 * @brief Send message to all objects.
 * @param aMessage Message to send.
 */
void ChemistryManager::SendMessage(Message const& aMessage)
{
}
