/*
 * MenuElement.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: jimbob
 */

#include "MenuElement.h"

MenuElement::MenuElement(std::string const &aFilename)
{
  mObject = new GameObject(nullptr, aFilename);
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
    mObject->AddComponent(transform);
  }
  ParseAdditionalData(aParser);
}
