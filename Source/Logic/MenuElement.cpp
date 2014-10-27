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
  ObjectManager* manager = LUABind::StaticGameApp::GetApp()->GET<ObjectManager>();
  mObject = new GameObject(manager, aFileName);
  mLayer = 0;
}

MenuElement::~MenuElement()
{
}

GameObject* MenuElement::GetObject()
{
  return mObject;
}

int MenuElement::GetLayer()
{
  return mLayer;
}

void MenuElement::SetLayer(int const aLayer)
{
  mLayer = aLayer;
}

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
