/*
 * MenuElement.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: jimbob
 */

#include "MenuElement.h"
#include "LUATypes.h"
#include "GraphicsManager.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "Menu.h"

MenuElement::MenuElement(Menu *aOwner, HashString const &aFileName, bool const aReplaceable) : mOwner(aOwner), mReplaceable(aReplaceable),
  mParent(nullptr), mChildren()
{
  mObject = mOwner->GetLevel()->CreateObjectDelayed(aFileName, "Menus");
}

MenuElement::~MenuElement()
{
  for(ElementIT it = mChildren.begin(); it != mChildren.end(); ++it)
  {
    (*it)->SetParent(mParent);
  }
  if(mParent)
  {
    mParent->RemoveChild(this);
  }
  mParent = nullptr;
  mChildren.clear();
}

/**
 * @brief Get owner of element.
 * @return Owner.
 */
Menu* MenuElement::GetOwner()
{
  return mOwner;
}

/**
 * @brief Get object associated with element.
 * @return Object
 */
GameObject* MenuElement::GetObject()
{
  return mObject;
}

/**
 * @brief Get whether or not object is replaceable (delete on next frame)
 * @return True is replaceable.
 */
bool MenuElement::IsReplaceable() const
{
  return mReplaceable;
}

/**
 * @brief Get parent element.
 * @return Parent.
 */
MenuElement* MenuElement::GetParent() const
{
  return mParent;
}

/**
 * @brief Get children elements.
 * @return Children.
 */
MenuElement::ElementContainer& MenuElement::GetChildren()
{
  return mChildren;
}

/**
 * @brief Get owner of element.
 * @param aOwner Owner
 */
void MenuElement::SetOwner(Menu* aOwner)
{
  mOwner = aOwner;
}

/**
 * @brief Set parent object.
 * @param aParent
 */
void MenuElement::SetParent(MenuElement *aParent)
{
  mParent = aParent;
}

/**
 * @brief Add child to element.
 * @param aObject Element to add.
 */
void MenuElement::AddChild(MenuElement* aObject)
{
  if(aObject->GetParent())
  {
    aObject->GetParent()->RemoveChild(aObject);
    aObject->GetParent()->GetObject()->RemoveChild(aObject->GetObject());
  }
  
  aObject->GetObject()->SetParent(GetObject());
  aObject->SetParent(this);
  mChildren.insert(aObject);
}

/**
 * @brief Remove child from element.
 * @param aObject Element to remove.
 */
void MenuElement::RemoveChild(MenuElement *aObject)
{
  aObject->SetParent(nullptr);
  aObject->GetObject()->SetParent(nullptr);
  mChildren.erase(aObject);
}

/**
 * @brief Helper to create an element.
 * @param aParser
 */
void MenuElement::ParseFile(Parser &aParser)
{
  if(aParser.Find("Transform"))
  {
    // Get Position, Scale, and Size
    Transform *transform = mObject->GET<Transform>();
    transform->Deserialize(aParser);
    transform->GetPosition().z = 0.99f;
  }
  ParseAdditionalData(aParser);
  
  // Automatically add surface to UI.
  if(mObject->HAS<Surface>())
  {
    Surface *surface = mObject->GET<Surface>();
    GraphicsManager *manager = surface->GetManager();
    manager->RemoveSurface(surface);
    manager->AddUISurface(surface);
  }
  
  if(!mReplaceable)
    mOwner->AddObject(this);
  else
    mOwner->AddReplaceableObject(this);
}
