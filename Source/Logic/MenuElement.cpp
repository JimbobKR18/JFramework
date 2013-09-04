/*
 * MenuElement.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: jimbob
 */

#include "MenuElement.h"
#include "LUATypes.h"

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
  // Borrowing the static game app from our LUA stuff
  GameApp* app = LUABind::StaticGameApp::GetApp();
  if(aParser.Find("Transform"))
  {
    // Get Position, Scale, and Size
    Transform *transform = new Transform();
    transform->Deserialize(aParser);
    mObject->AddComponent(transform);
  }
  if(aParser.Find("Surface"))
  {
#if !defined(ANDROID) && !defined(IOS)
    PCSurface *surface = (PCSurface*)app->GET<GraphicsManager>()->CreateSurface();
    std::string imageName = aParser.Find("Surface", "TextureName");
    surface->LoadImage(imageName);
#else
    Surface *surface = new Surface();
#endif
    surface->Deserialize(aParser);
    mObject->AddComponent(surface);
  }
}
