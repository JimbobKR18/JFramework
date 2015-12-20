/*
 * MenuElement.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: jimbob
 */

#include "MenuElement.h"
#include "LUATypes.h"
#include "ObjectManager.h"

MenuElement::MenuElement(std::string const &aFileName)
{
  mManager = LUABind::StaticGameApp::GetApp()->GET<ObjectManager>();
  mObject = new GameObject(mManager, aFileName);
  mLayer = 0;
}

MenuElement::~MenuElement()
{
  delete mObject;
}

/**
 * @brief Get object associated with element.
 * @return 
 */
GameObject* MenuElement::GetObject()
{
  return mObject;
}

/**
 * @brief Get view layer associated with this element
 * @return 
 */
int MenuElement::GetLayer()
{
  return mLayer;
}

/**
 * @brief Set view layer.
 * @param aLayer
 */
void MenuElement::SetLayer(int const aLayer)
{
  mLayer = aLayer;
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
    Transform *transform = new Transform();
    transform->Deserialize(aParser);
    transform->GetPosition().z = 0.99f;
    mObject->AddComponent(transform);
  }
  ParseAdditionalData(aParser);
}
