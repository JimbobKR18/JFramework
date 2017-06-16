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
#include "EffectsManager.h"
#include "CustomScript.h"

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
 * @brief Retrieve an element if available. Uses file name if ".txt" or ".xml" is in name.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElement(HashString const &aName) const
{
  if(aName.Find(".txt") || aName.Find(".xml"))
  {
    return GetElementByFileName(aName);
  }
  else
  {
    return GetElementByObjectName(aName);
  }
}

/**
 * @brief Retrieve an element by object name if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElementByObjectName(HashString const &aObjectName) const
{
  for(MenuElement::ConstElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if((*it)->GetObject()->GetName() == aObjectName)
    {
      return *it;
    }
  }
  return nullptr;
}

/**
 * @brief Retrieve an element by file name if available.
 * @param aFileName
 * @return The element specified, else nullptr.
 */
MenuElement* Menu::GetElementByFileName(HashString const &aFileName) const
{
  for(MenuElement::ConstElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if((*it)->GetObject()->GetFileName() == aFileName)
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
MenuElement::ElementContainer Menu::GetElements() const
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
  
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aElement == *it)
    {
      return;
    }
  }
  
  mMenuElements.insert(aElement);
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
  
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if(aElement == *it)
    {
      return;
    }
  }
  
  mReplaceableElements.insert(aElement);
  aElement->SetOwner(this);
}

/**
 * @brief Deletes and removes an element.
 * @param aElement
 */
void Menu::DeleteObject(MenuElement *aElement)
{
  // Using delayed delete
  DeleteElementChildren(aElement);
}

/**
 * @brief Delete all objects, the right way.
 */
void Menu::DeleteObjects()
{
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end();)
  {
    DeleteObject(*it);
    it = mMenuElements.begin();
  }
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end();)
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
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end();)
  {
    DeleteObject(*it);
    it = mReplaceableElements.begin();
  }
  mReplaceableElements.clear();
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
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
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
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
    HashString name;
    
    // Compatibility layer
    if(newElement->Find("Name"))
      name = newElement->Find("Name")->GetValue();
    else
      name = newElement->Find("File")->GetValue();
    
    if(newElement->Find("Type")->GetValue().ToString() == "Image")
      element = new MenuImage(this, name, false);
    else if(newElement->Find("Type")->GetValue().ToString() == "Text")
    {
      if(newElement->Find("Text"))
        element = new MenuText(this, name, newElement->Find("Text")->GetValue(), false);
      else
        element = new MenuText(this, name, "", false);
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
    if(newElement->Find("CustomScript"))
    {
      ParseCustomScript(element->GetObject(), newElement->Find("CustomScript"));
    }
    if(newElement->Find("ObjectName"))
    {
      element->GetObject()->SetName(newElement->Find("ObjectName")->GetValue());
    }
    if(newElement->Find("Effects"))
    {
      ParseEffects(element->GetObject(), newElement->Find("Effects"));
    }
    if(newElement->Find("Tags"))
    {
      if(newElement->Find("Tags")->Find("Clear") && newElement->Find("Tags")->Find("Clear")->GetValue().ToBool())
        element->GetObject()->ClearTags();
      
      std::vector<HashString> tags = newElement->Find("Tags")->GetValue().ToHashStringVector();
      for(std::vector<HashString>::iterator it = tags.begin(); it != tags.end(); ++it)
      {
        element->GetObject()->AddTag(*it);
      }
    }
    if(newElement->Find("Parent"))
    {
      HashString parentName = newElement->Find("Parent")->GetValue();
      MenuElement *parent = GetElement(parentName);
      
      if(!parent)
        assert(!"Parent not found.");
      
      parent->GetObject()->AddChild(element->GetObject());
      element->GetObject()->SetParent(parent->GetObject());
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
  
  // Axis lock (optional)
  Root* axisLockRoot = aTransform->Find("LockedAxes");
  if(axisLockRoot)
  {
    AxisLock axisLock = NO_AXIS;
    HashString axisLockString = axisLockRoot->GetValue();
    if(axisLockString == "X_AXIS")
      axisLock = X_AXIS;
    else if(axisLockString == "Y_AXIS")
      axisLock = Y_AXIS;
    else if(axisLockString == "Z_AXIS")
      axisLock = Z_AXIS;
    else if(axisLockString == "XY_AXIS")
      axisLock = XY_AXIS;
    else if(axisLockString == "YZ_AXIS")
      axisLock = YZ_AXIS;
    else if(axisLockString == "XZ_AXIS")
      axisLock = XZ_AXIS;
    else if(axisLockString == "ALL_AXES")
      axisLock = ALL_AXES;
    else if(axisLockString == "NO_AXIS")
      axisLock = NO_AXIS;
    else
      assert(!"Invalid axis lock value passed in.");
    objTransform->SetLockedAxis(axisLock);
  }
  
  // Parent inherit info
  Root* inheritNode = aTransform->Find("InheritInfo");
  if(inheritNode)
  {
    ParentInherit inheritance = INHERIT_ALL;
    HashString inheritInfo = inheritNode->GetValue();
    if(inheritInfo == "INHERIT_NONE")
      inheritance = INHERIT_NONE;
    else if(inheritInfo == "INHERIT_POSITION")
      inheritance = INHERIT_POSITION;
    else if(inheritInfo == "INHERIT_ROTATION")
      inheritance = INHERIT_ROTATION;
    else if(inheritInfo == "INHERIT_SCALE")
      inheritance = INHERIT_SCALE;
    else if(inheritInfo == "INHERIT_POSITION_ROTATION")
      inheritance = INHERIT_POSITION_ROTATION;
    else if(inheritInfo == "INHERIT_ROTATION_SCALE")
      inheritance = INHERIT_ROTATION_SCALE;
    else if(inheritInfo == "INHERIT_POSITION_SCALE")
      inheritance = INHERIT_POSITION_SCALE;
    else if(inheritInfo == "INHERIT_ALL")
      inheritance = INHERIT_ALL;
    else
      assert(!"Invalid inheritance value passed in.");
    objTransform->SetParentInheritanceInfo(inheritance);
  }
}

/**
 * @brief Get surface data from a root.
 * @param aSurface
 */
void Menu::ParseSurface(GameObject *aObject, Root *aSurface)
{
  Surface* objSurface = aObject->GET<Surface>();
  int startingAnimation = 0;
  float r, g, b, a;
  
  if(aSurface->Find("ColorR"))
  {
    r = aSurface->Find("ColorR")->GetValue().ToFloat();
    g = aSurface->Find("ColorG")->GetValue().ToFloat();
    b = aSurface->Find("ColorB")->GetValue().ToFloat();
    a = aSurface->Find("ColorA")->GetValue().ToFloat();
    objSurface->SetColor(Vector4(r, g, b, a));
  }
  if(aSurface->Find("StartingAnimation"))
  {
    startingAnimation = aSurface->Find("StartingAnimation")->GetValue().ToInt();
    objSurface->SetAnimation(startingAnimation);
  }
  if(aSurface->Find("ViewMode"))
  {
    HashString viewMode = aSurface->Find("ViewMode")->GetValue();
    if(viewMode == "ABSOLUTE")
      objSurface->SetViewMode(VIEW_ABSOLUTE);
    else if(viewMode == "RELATIVE")
      objSurface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
    else if(viewMode == "PERCENTAGE")
      objSurface->SetViewMode(VIEW_PERCENTAGE_OF_CAMERA);
    else
      assert(!"Invalid value passed into ViewMode for Surface. (Menu.cpp)");  
  }
#ifdef SHADER_COMPATIBLE
  if(aSurface->Find("VertexShader") && aSurface->Find("FragmentShader"))
  {
    objSurface->LoadShaders(aSurface->Find("VertexShader")->GetValue(), aSurface->Find("FragmentShader")->GetValue());
  }
#endif
}

/**
 * @brief Parse effects root.
 * @param aObject Object to apply effects to.
 * @param aEffects Effects root.
 */
void Menu::ParseEffects(GameObject *aObject, Root *aEffects)
{
  EffectsManager* effectsManager = mOwner->GetManager()->GetOwningApp()->GET<EffectsManager>();
  int curIndex = 0;
  HashString const effectString = "Effect_";
  HashString curEffect = effectString + Common::IntToString(curIndex);
  while(aEffects->Find(curEffect))
  {
    Root* effectRoot = aEffects->Find(curEffect);
    HashString type = effectRoot->Find("Type")->GetValue();
    HashString name = effectRoot->Find("Name")->GetValue();
    float time = effectRoot->Find("Time")->GetValue().ToFloat();
    Effect* effect = effectsManager->CreateEffect(type);
    effect->SetName(name);
    effect->SetObject(aObject);
    effect->SetTime(time);
    effect->Deserialize(effectRoot);
    ++curIndex;
    curEffect = effectString + Common::IntToString(curIndex);
  }
}

/**
 * @brief Parse custom script root.
 * @param aObject Object to apply effects to.
 * @param aCustomScript Custom script root.
 */
void Menu::ParseCustomScript(GameObject *aObject, Root *aCustomScript)
{
  CustomScript *customScript = aObject->GET<CustomScript>();
  if(!customScript)
  {
    customScript = new CustomScript();
    aObject->AddComponent(customScript);
  }
  
  if(aCustomScript->Find("FileName"))
    customScript->SetFileName(aCustomScript->Find("FileName")->GetValue());
  if(aCustomScript->Find("UpdateFunctionName"))
    customScript->SetUpdateFunctionName(aCustomScript->Find("UpdateFunctionName")->GetValue());
}

/**
 * @brief Delete element and its children.
 * @param aElement Element to delete.
 */
void Menu::DeleteElementChildren(MenuElement *aElement)
{
  mMenuElements.erase(aElement);
  mReplaceableElements.erase(aElement);
  mOwner->DeleteObjectDelayed(aElement->GetObject());
  delete aElement;
}

/**
 * @brief Remove elements based on object, doesn't delete object.
 * @param aObject Object to search for and remove.
 */
void Menu::ShallowRemoveElementForObject(GameObject *aObject)
{
  for(MenuElement::ElementIT it = mMenuElements.begin(); it != mMenuElements.end(); ++it)
  {
    if(aObject == (*it)->GetObject())
    {
      ShallowRemoveElementChildren(*it);
      return;
    }
  }
  for(MenuElement::ElementIT it = mReplaceableElements.begin(); it != mReplaceableElements.end(); ++it)
  {
    if(aObject == (*it)->GetObject())
    {
      ShallowRemoveElementChildren(*it);
      return;
    }
  }
}

/**
 * @brief Remove element children based on object, doesn't delete object.
 * @param aElement Object to search for and remove.
 * @param aReplaceable What grouping the object belongs to.
 */
void Menu::ShallowRemoveElementChildren(MenuElement *aElement)
{
  mMenuElements.erase(aElement);
  mReplaceableElements.erase(aElement);
  
  // Don't delete object associated.
  delete aElement;
}
