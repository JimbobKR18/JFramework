/*
 * MenuElement.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: jimbob
 */

#include "MenuElement.h"
#include "LUATypes.h"
#include "LevelManager.h"
#include "ObjectManager.h"

MenuElement::MenuElement(std::string const &aFileName)
{
  mManager = LUABind::StaticGameApp::GetApp()->GET<LevelManager>();
  mObject = mManager->GetActiveLevel()->CreateObjectDelayed(aFileName, "Menus");
  mLayer = 0;
}

MenuElement::~MenuElement()
{
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
    Transform *transform = mObject->GET<Transform>();
    transform->Deserialize(aParser);
    transform->GetPosition().z = 0.99f;
  }
  ParseAdditionalData(aParser);
}
