/*
 * Menu.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#include "Menu.h"

Menu::Menu(Level *aLevel, std::string const &aFilename) : mOwner(aLevel), mFilename(aFilename)
{
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

void Menu::AddObject(GameObject *aObject)
{
  mObjects.push_back(aObject);
  mOwner->AddObject(aObject);
}
void Menu::DeleteObject(GameObject *aObject)
{
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    if(aObject == *it)
    {
      mObjects.erase(it);
      mOwner->DeleteObject(aObject);
      break;
    }
  }
}
void Menu::DeleteObjects()
{
  for(ObjectIT it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    DeleteObject(*it);
  }
}
