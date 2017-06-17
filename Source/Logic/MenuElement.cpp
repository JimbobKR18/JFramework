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

MenuElement::MenuElement(Menu *aOwner, HashString const &aFileName, bool const aReplaceable) : mOwner(aOwner), mReplaceable(aReplaceable)
{
  mObject = mOwner->GetLevel()->CreateObjectDelayed(aFileName, "Menus");
}

MenuElement::~MenuElement()
{
  mObject = nullptr;
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
 * @brief Get owner of element.
 * @param aOwner Owner
 */
void MenuElement::SetOwner(Menu* aOwner)
{
  mOwner = aOwner;
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
