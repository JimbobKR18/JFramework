/*
 * Menu.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#include "Menu.h"
#include "MenuImage.h"

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

void Menu::AddObject(MenuElement *aElement)
{
  mMenuElements.push_back(aElement);
  mOwner->AddObject(aElement->GetObject());
}
void Menu::DeleteObject(MenuElement *aElement)
{
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aElement == *it)
    {
      mMenuElements.erase(it);
      mOwner->DeleteObject(aElement->GetObject());
      break;
    }
  }
  delete aElement;
}
void Menu::DeleteObjects()
{
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    DeleteObject(*it);
  }
}

void Menu::ParseFile()
{
  TextParser parser(Common::RelativePath(mFilename).c_str(), false);
  
	while(parser.IsGood())
	{
		std::string param, type;
		parser.GetNextString(param);
    
		if(param.length() == 0)
			break;
    
    parser.GetNextString(type);
    
    MenuElement *element;
    
    if(type == "Image")
      element = new MenuImage(param);
    /*else
      element = new MenuText(param);*/
      
    AddObject(element);
	}
}
