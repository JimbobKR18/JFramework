/*
 * Menu.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#include "Menu.h"
#include "MenuImage.h"
#include "MenuText.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "EffectsManager.h"
#include "CustomScript.h"

Menu::Menu(Level *aLevel, HashString const &aFileName) : mOwner(aLevel), mFileName(aFileName), mMenuElements(), mReplaceableElements()
{
  mName = Common::RetrieveNameFromFileName(aFileName);
  
  ParseFile();
}
Menu::~Menu()
{
  DeleteObjects();
}

/**
 * @brief Get name of menu
 * @return Name of menu
 */
HashString Menu::GetName() const
{
  return mName;
}

/**
 * @brief Get file name for menu
 * @return File name for menu
 */
HashString Menu::GetFileName() const
{
  return mFileName;
}

/**
 * @brief Get level of menu
 * @return Level of menu
 */
Level* Menu::GetLevel() const
{
  return mOwner;
}

/**
 * @brief Retrieve an element if available. Uses file name if ".txt" or ".xml" is in name.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElement(HashString const &aName) const
{
  if(aName.Find(".txt") || aName.Find(".xml"))
  {
    return GetElementByFileName(aName);
  }
  else
  {
    return GetElementByObjectName(aName);
  }
}

/**
 * @brief Retrieve an element by object name if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElementByObjectName(HashString const &aObjectName) const
{
  for(MenuElement::ConstElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if((*it)->GetObject()->GetName() == aObjectName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Retrieve an element by file name if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElementByFileName(HashString const &aFileName) const
{
  for(MenuElement::ConstElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if((*it)->GetObject()->GetFileName() == aFileName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Get all elements in menu.
 * @return All elements in menu.
 */
MenuElement::ElementContainer Menu::GetElements() const
{
  return mMenuElements;
}

/**
 * @brief Retrieve a replaceable element if available. Uses file name if ".txt" or ".xml" is in name.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetReplaceableElement(HashString const &aName) const
{
  if(aName.Find(".txt") || aName.Find(".xml"))
  {
    return GetReplaceableElementByFileName(aName);
  }
  else
  {
    return GetReplaceableElementByObjectName(aName);
  }
}

/**
 * @brief Retrieve a replaceable element by object name if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetReplaceableElementByObjectName(HashString const &aObjectName) const
{
  for(MenuElement::ConstElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if((*it)->GetObject()->GetName() == aObjectName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Retrieve a replaceable element by file name if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetReplaceableElementByFileName(HashString const &aFileName) const
{
  for(MenuElement::ConstElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if((*it)->GetObject()->GetFileName() == aFileName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Get all replaceable elements in menu.
 * @return All elements in menu.
 */
MenuElement::ElementContainer Menu::GetReplaceableElements() const
{
  return mReplaceableElements;
}

/**
 * @brief Adds an element to our menu.
 * @param aElement
 */
void Menu::AddObject(MenuElement *aElement)
{
  if(aElement && aElement->IsReplaceable())
    assert(!"Replaceable element placed in regular elements, will cause error.");
  
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aElement == *it)
    {
      return;
    }
  }
  
  mMenuElements.insert(aElement);
  aElement->SetOwner(this);
}

/**
 * @brief Adds an element to our menu. To be deleted next frame.
 * @param aElement
 */
void Menu::AddReplaceableObject(MenuElement *aElement)
{
  if(aElement && !aElement->IsReplaceable())
    assert(!"Regular element placed in replaceable elements, will cause error.");
  
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if(aElement == *it)
    {
      return;
    }
  }
  
  mReplaceableElements.insert(aElement);
  aElement->SetOwner(this);
}

/**
 * @brief Deletes and removes an element.
 * @param aElement
 */
void Menu::DeleteObject(MenuElement *aElement)
{
  // Using delayed delete
  DeleteElementChildren(aElement);
}

/**
 * @brief Delete all objects, the right way.
 */
void Menu::DeleteObjects()
{
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end();)
  {
    DeleteObject(*it);
    it = mMenuElements.begin();
  }
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end();)
  {
    DeleteObject(*it);
    it = mReplaceableElements.begin();
  }
  mMenuElements.clear();
  mReplaceableElements.clear();
}

/**
 * @brief Updates menu elements if need be.
 */
void Menu::Update()
{
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end();)
  {
    DeleteObject(*it);
    it = mReplaceableElements.begin();
  }
  mReplaceableElements.clear();
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    (*it)->Update();
  }
}

/**
 * @brief Sends a message to all menu elements.
 * @param aMessage
 */
void Menu::ReceiveMessage(Message const& aMessage)
{
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    (*it)->ReceiveMessage(aMessage);
  }
}

/**
 * @brief Does nothing for now.
 * @param aMessage
 */
void Menu::SendMessage(Message const& aMessage)
{
}

/**
 * @brief Helper function to parse a text or xml (or whatever) file.
 */
void Menu::ParseFile()
{
  TextParser parser(Common::RelativePath("Menus", mFileName).c_str());

  MenuElement *element = nullptr;
  HashString object = "Object_";
  int curIndex = 0;
  HashString curObject = object + Common::IntToString(curIndex);

  while(parser.Find(curObject.ToString()))
  {
    ParserNode* newElement = parser.Find(curObject.ToString());
    HashString name;
    
    // Compatibility layer
    if(newElement->Find("Name"))
      name = newElement->Find("Name")->GetValue();
    else
      name = newElement->Find("File")->GetValue();
    
    if(newElement->Find("Type")->GetValue().ToString() == "Image")
      element = new MenuImage(this, name, false);
    else if(newElement->Find("Type")->GetValue().ToString() == "Text")
    {
      if(newElement->Find("Text"))
        element = new MenuText(this, name, newElement->Find("Text")->GetValue(), false);
      else
        element = new MenuText(this, name, "", false);
    }
    else
      assert(!"Invalid MenuElement passed into menu");
      
    if(newElement->Find("Transform"))
    {
      ParseTransform(element->GetObject(), newElement->Find("Transform"));
    }
    if(newElement->Find("Surface"))
    {
      ParseSurface(element->GetObject(), newElement->Find("Surface"));
    }
    if(newElement->Find("CustomScript"))
    {
      ParseCustomScript(element->GetObject(), newElement->Find("CustomScript"));
    }
    if(newElement->Find("ObjectName"))
    {
      element->GetObject()->SetName(newElement->Find("ObjectName")->GetValue());
    }
    if(newElement->Find("Effects"))
    {
      ParseEffects(element->GetObject(), newElement->Find("Effects"));
    }
    if(newElement->Find("Tags"))
    {
      if(newElement->Find("Tags")->Find("Clear") && newElement->Find("Tags")->Find("Clear")->GetValue().ToBool())
        element->GetObject()->ClearTags();
      
      std::vector<HashString> tags = newElement->Find("Tags")->GetValue().ToHashStringVector();
      for(std::vector<HashString>::iterator it = tags.begin(); it != tags.end(); ++it)
      {
        element->GetObject()->AddTag(*it);
      }
    }
    if(newElement->Find("Parent"))
    {
      HashString parentName = newElement->Find("Parent")->GetValue();
      MenuElement *parent = GetElement(parentName);
      
      if(!parent)
        assert(!"Parent not found.");
      
      parent->GetObject()->AddChild(element->GetObject());
      element->GetObject()->SetParent(parent->GetObject());
    }

    AddObject(element);

    ++curIndex;
    curObject = object + Common::IntToString(curIndex);
  }

  // Add our menu to the level
  mOwner->AddMenu(this);
}

/**
 * @brief Get transform data for an object from a root.
 * @param aTransform
 */
void Menu::ParseTransform(GameObject *aObject, ParserNode *aTransform)
{
  Transform* objTransform = aObject->GET<Transform>();
  objTransform->Deserialize(aTransform);
}

/**
 * @brief Get surface data from a root.
 * @param aSurface
 */
void Menu::ParseSurface(GameObject *aObject, ParserNode *aSurface)
{
  Surface* objSurface = aObject->GET<Surface>();
  objSurface->Deserialize(aSurface);
}

/**
 * @brief Parse effects root.
 * @param aObject Object to apply effects to.
 * @param aEffects Effects root.
 */
void Menu::ParseEffects(GameObject *aObject, ParserNode *aEffects)
{
  EffectsManager* effectsManager = mOwner->GetManager()->GetOwningApp()->GET<EffectsManager>();
  int curIndex = 0;
  HashString const effectString = "Effect_";
  HashString curEffect = effectString + Common::IntToString(curIndex);
  while(aEffects->Find(curEffect))
  {
    ParserNode* effectRoot = aEffects->Find(curEffect);
    HashString type = effectRoot->Find("Type")->GetValue();
    HashString name = effectRoot->Find("Name")->GetValue();
    float time = effectRoot->Find("Time")->GetValue().ToFloat();
    Effect* effect = effectsManager->CreateEffect(type);
    effect->SetName(name);
    effect->SetObject(aObject);
    effect->SetTime(time);
    effect->Deserialize(effectRoot);
    ++curIndex;
    curEffect = effectString + Common::IntToString(curIndex);
  }
}

/**
 * @brief Parse custom script root.
 * @param aObject Object to apply effects to.
 * @param aCustomScript Custom script root.
 */
void Menu::ParseCustomScript(GameObject *aObject, ParserNode *aCustomScript)
{
  CustomScript *customScript = aObject->GET<CustomScript>();
  if(!customScript)
  {
    customScript = new CustomScript();
    aObject->AddComponent(customScript);
  }
  
  customScript->Deserialize(aCustomScript);
}

/**
 * @brief Delete element and its children.
 * @param aElement Element to delete.
 */
void Menu::DeleteElementChildren(MenuElement *aElement)
{
  mMenuElements.erase(aElement);
  mReplaceableElements.erase(aElement);
  mOwner->DeleteObjectDelayed(aElement->GetObject());
  delete aElement;
}

/**
 * @brief Remove elements based on object, doesn't delete object.
 * @param aObject Object to search for and remove.
 */
void Menu::ShallowRemoveElementForObject(GameObject *aObject)
{
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aObject == (*it)->GetObject())
    {
      ShallowRemoveElementChildren(*it);
      return;
    }
  }
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if(aObject == (*it)->GetObject())
    {
      ShallowRemoveElementChildren(*it);
      return;
    }
  }
}

/**
 * @brief Remove element children based on object, doesn't delete object.
 * @param aElement Object to search for and remove.
 * @param aReplaceable What grouping the object belongs to.
 */
void Menu::ShallowRemoveElementChildren(MenuElement *aElement)
{
  mMenuElements.erase(aElement);
  mReplaceableElements.erase(aElement);
  
  // Don't delete object associated.
  delete aElement;
}
