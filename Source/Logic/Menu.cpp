/*
 * Menu.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#include "Menu.h"
#include "MenuImage.h"
#include "MenuText.h"

Menu::Menu(Level *aLevel, std::string const &aFilename) : mOwner(aLevel), mFilename(aFilename)
{
  ParseFile();
}
Menu::~Menu()
{
  DeleteObjects();
}

std::string Menu::GetName()
{
  return mFilename;
}
Level* Menu::GetLevel()
{
  return mOwner;
}

void Menu::AddObject(MenuElement *aElement)
{
  mMenuElements.push_back(aElement);
  mOwner->AddObject(aElement->GetObject());
}
void Menu::DeleteObject(MenuElement *aElement)
{
  // Using delayed delete
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aElement == *it)
    {
      mMenuElements.erase(it);
      mOwner->DeleteObjectDelayed(aElement->GetObject());
      break;
    }
  }
  delete aElement;
}
void Menu::DeleteObjects()
{
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end();)
  {
    DeleteObject(*it);
    it = mMenuElements.begin();
  }
  mMenuElements.clear();
}

void Menu::ParseFile()
{
  TextParser parser(Common::RelativePath("Menus", mFilename).c_str());

  MenuElement *element = nullptr;
  HashString object = "Object_";
  int curIndex = 0;
  HashString curObject = object + Common::IntToString(curIndex);


  while(parser.Find(curObject.ToString()))
  {
    Root* newElement = parser.Find(curObject.ToString());
    if(newElement->Find("Type")->GetValue().ToString() == "Image")
      element = new MenuImage(newElement->Find("Name")->GetValue().ToString());
    else if(newElement->Find("Type")->GetValue().ToString() == "Text")
      element = new MenuText(newElement->Find("Name")->GetValue().ToString());
    else
      assert(!"Invalid MenuElement passed into menu");

    if(element)
      AddObject(element);

    ++curIndex;
    curObject = object + Common::IntToString(curIndex);
  }

  // Add our menu to the level
  mOwner->AddMenu(this);
}
