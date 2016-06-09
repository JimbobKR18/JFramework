/*
 * Menu.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: jimbob
 */

#include "Menu.h"
#include "MenuImage.h"
#include "MenuText.h"
#include "ObjectManager.h"
#include "LevelManager.h"

Menu::Menu(Level *aLevel, HashString const &aFileName) : mOwner(aLevel), mFileName(aFileName), mMenuElements(), mReplaceableElements()
{
  mName = Common::RetrieveNameFromFileName(aFileName);
  
  ParseFile();
}
Menu::~Menu()
{
  DeleteObjects();
}

/**
 * @brief Get name of menu
 * @return Name of menu
 */
HashString Menu::GetName() const
{
  return mName;
}

/**
 * @brief Get file name for menu
 * @return File name for menu
 */
HashString Menu::GetFileName() const
{
  return mFileName;
}

/**
 * @brief Get level of menu
 * @return Level of menu
 */
Level* Menu::GetLevel() const
{
  return mOwner;
}

/**
 * @brief Retrieve an element if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElement(HashString const &aFileName) const
{
  for(ConstElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if((*it)->GetObject()->GetFileName() == aFileName.ToString())
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Get all elements in menu.
 * @return All elements in menu.
 */
Menu::ElementContainer Menu::GetElements() const
{
  return mMenuElements;
}

/**
 * @brief Adds an element to our menu.
 * @param aElement
 */
void Menu::AddObject(MenuElement *aElement)
{
  if(aElement && aElement->IsReplaceable())
    assert(!"Replaceable element placed in regular elements, will cause error.");
  
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aElement == *it)
    {
      return;
    }
  }
  
  mMenuElements.push_back(aElement);
  aElement->SetOwner(this);
}

/**
 * @brief Adds an element to our menu. To be deleted next frame.
 * @param aElement
 */
void Menu::AddReplaceableObject(MenuElement *aElement)
{
  if(aElement && !aElement->IsReplaceable())
    assert(!"Regular element placed in replaceable elements, will cause error.");
  
  for(ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if(aElement == *it)
    {
      return;
    }
  }
  
  mReplaceableElements.push_back(aElement);
  aElement->SetOwner(this);
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
      delete aElement;
      return;
    }
  }
  for(ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if(aElement == *it)
    {
      mReplaceableElements.erase(it);
      mOwner->DeleteObjectDelayed(aElement->GetObject());
      delete aElement;
      return;
    }
  }
  assert(!"MenuElement not found.");
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
  for(ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end();)
  {
    DeleteObject(*it);
    it = mReplaceableElements.begin();
  }
  mMenuElements.clear();
  mReplaceableElements.clear();
}

/**
 * @brief Updates menu elements if need be.
 */
void Menu::Update()
{
  for(ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end();)
  {
    DeleteObject(*it);
    it = mReplaceableElements.begin();
  }
  mReplaceableElements.clear();
  for(ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    (*it)->Update();
  }
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
      element = new MenuImage(this, newElement->Find("Name")->GetValue(), false);
    else if(newElement->Find("Type")->GetValue().ToString() == "Text")
    {
      if(newElement->Find("Text"))
        element = new MenuText(this, newElement->Find("Name")->GetValue(), newElement->Find("Text")->GetValue(), false);
      else
        element = new MenuText(this, newElement->Find("Name")->GetValue(), "", false);
    }
    else
      assert(!"Invalid MenuElement passed into menu");
      
    if(newElement->Find("Transform"))
    {
      ParseTransform(element->GetObject(), newElement->Find("Transform"));
    }
    if(newElement->Find("Surface"))
    {
      ParseSurface(element->GetObject(), newElement->Find("Surface"));
    }

    AddObject(element);

    ++curIndex;
    curObject = object + Common::IntToString(curIndex);
  }

  // Add our menu to the level
  mOwner->AddMenu(this);
}

/**
 * @brief Get transform data for an object from a root.
 * @param aTransform
 */
void Menu::ParseTransform(GameObject *aObject, Root *aTransform)
{
  float posX, posY, posZ,
      scaleX, scaleY, scaleZ,
      sizeX, sizeY, sizeZ,
      rotateX, rotateY, rotateZ;
  posX = aTransform->Find("PositionX")->GetValue().ToFloat();
  posY = aTransform->Find("PositionY")->GetValue().ToFloat();
  posZ = aTransform->Find("PositionZ")->GetValue().ToFloat();
  scaleX = aTransform->Find("ScaleX")->GetValue().ToFloat();
  scaleY = aTransform->Find("ScaleY")->GetValue().ToFloat();
  scaleZ = aTransform->Find("ScaleZ")->GetValue().ToFloat();
  sizeX = aTransform->Find("SizeX")->GetValue().ToFloat();
  sizeY = aTransform->Find("SizeY")->GetValue().ToFloat();
  sizeZ = aTransform->Find("SizeZ")->GetValue().ToFloat();
  rotateX = aTransform->Find("RotationX")->GetValue().ToFloat();
  rotateY = aTransform->Find("RotationY")->GetValue().ToFloat();
  rotateZ = aTransform->Find("RotationZ")->GetValue().ToFloat();

  Transform* objTransform = aObject->GET<Transform>();
  objTransform->SetPosition(Vector3(posX,posY,posZ));
  objTransform->SetScale(Vector3(scaleX,scaleY,scaleZ));
  objTransform->SetSize(Vector3(sizeX,sizeY,sizeZ));
  
  Matrix33 rotation;
  rotation = rotation.Rotate(Vector3(1,0,0), rotateX);
  rotation = rotation.Rotate(Vector3(0,1,0), rotateY);
  rotation = rotation.Rotate(Vector3(0,0,1), rotateZ);
  objTransform->SetRotation(rotation);
  
  // Alignment (optional)
  if(aTransform->Find("AlignX"))
  {
    objTransform->SetXAlignment(X_ALIGN_CENTER);
    if(aTransform->Find("AlignX")->GetValue() == "LEFT")
      objTransform->SetXAlignment(X_ALIGN_LEFT);
    else if(aTransform->Find("AlignX")->GetValue() == "RIGHT")
      objTransform->SetXAlignment(X_ALIGN_RIGHT);
    else if(aTransform->Find("AlignX")->GetValue() != "CENTER")
      assert(!"Invalid value passed into XAlign");
  }
  if(aTransform->Find("AlignY"))
  {
    objTransform->SetYAlignment(Y_ALIGN_CENTER);
    if(aTransform->Find("AlignY")->GetValue() == "TOP")
      objTransform->SetYAlignment(Y_ALIGN_TOP);
    else if(aTransform->Find("AlignY")->GetValue() == "BOTTOM")
      objTransform->SetYAlignment(Y_ALIGN_BOTTOM);
    else if(aTransform->Find("AlignY")->GetValue() != "CENTER")
      assert(!"Invalid value passed into YAlign");
  }
  if(aTransform->Find("AlignZ"))
  {
    objTransform->SetZAlignment(Z_ALIGN_CENTER);
    if(aTransform->Find("AlignZ")->GetValue() == "FRONT")
      objTransform->SetZAlignment(Z_ALIGN_FRONT);
    else if(aTransform->Find("AlignZ")->GetValue() == "BACK")
      objTransform->SetZAlignment(Z_ALIGN_BACK);
    else if(aTransform->Find("AlignZ")->GetValue() != "CENTER")
      assert(!"Invalid value passed into ZAlign");
  }
}

/**
 * @brief Get surface data from a root.
 * @param aSurface
 */
void Menu::ParseSurface(GameObject *aObject, Root *aSurface)
{
  int startingAnimation = 0;
  float r, g, b, a;
  r = aSurface->Find("ColorR")->GetValue().ToFloat();
  g = aSurface->Find("ColorG")->GetValue().ToFloat();
  b = aSurface->Find("ColorB")->GetValue().ToFloat();
  a = aSurface->Find("ColorA")->GetValue().ToFloat();
  startingAnimation = aSurface->Find("StartingAnimation")->GetValue().ToInt();

  Surface* objSurface = aObject->GET<Surface>();
  objSurface->SetColor(Vector4(r, g, b, a));
  objSurface->SetAnimation(startingAnimation);
  
  if(aSurface->Find("VertexShader") && aSurface->Find("FragmentShader"))
  {
    objSurface->LoadShaders(aSurface->Find("VertexShader")->GetValue(), aSurface->Find("FragmentShader")->GetValue());
  }
}