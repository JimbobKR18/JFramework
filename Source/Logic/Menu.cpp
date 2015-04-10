/*
 * Menu.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#include "Menu.h"
#include "MenuImage.h"
#include "MenuText.h"

Menu::Menu(Level *aLevel, std::string const &aFileName) : mOwner(aLevel), mFileName(aFileName)
{
  mName = Common::RetrieveNameFromFileName(aFileName);
  
  ParseFile();
}
Menu::~Menu()
{
  DeleteObjects();
}

HashString Menu::GetName()
{
  return mName;
}
HashString Menu::GetFileName()
{
  return mFileName;
}
Level* Menu::GetLevel()
{
  return mOwner;
}

/**
 * @brief Retrieve an element if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElement(HashString const &aFileName)
{
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if((*it)->GetObject()->GetFileName() == aFileName.ToString())
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Adds an element to our menu.
 * @param aElement
 */
void Menu::AddObject(MenuElement *aElement)
{
  mMenuElements.push_back(aElement);
  mOwner->AddObject(aElement->GetObject());
}

/**
 * @brief Deletes and removes an element.
 * @param aElement
 */
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

/**
 * @brief Delete all objects, the right way.
 */
void Menu::DeleteObjects()
{
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end();)
  {
    DeleteObject(*it);
    it = mMenuElements.begin();
  }
  mMenuElements.clear();
}

/**
 * @brief Sends a message to all menu elements.
 * @param aMessage
 */
void Menu::ReceiveMessage(Message const& aMessage)
{
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
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
