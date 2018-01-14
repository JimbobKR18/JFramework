//
//  MenuImage.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 3/26/13.
//
//

#if !defined(IOS) && !defined(ANDROID)
#define SHADER_COMPATIBLE
#endif

#include "MenuImage.h"
#include "LUATypes.h"
#include "GraphicsManager.h"

#ifdef SHADER_COMPATIBLE
  #include "PCShaderSurface.h"
#else
#endif

MenuImage::MenuImage(Menu *aOwner, HashString const &aFilename, bool const aReplaceable) : MenuElement(aOwner, aFilename, aReplaceable)
{
  TextParser parser(Common::RelativePath("Menus", aFilename));
  ParseFile(parser);
}

MenuImage::~MenuImage()
{
}

/**
 * @brief Does nothing.
 */
void MenuImage::Draw()
{
}

/**
 * @brief Does nothing.
 */
void MenuImage::Update()
{
}

/**
 * @brief Does nothing.
 * @param aMessage
 */
void MenuImage::SendMessage(Message const &aMessage)
{
}

/**
 * @brief Does nothing.
 * @param aMessage
 */
void MenuImage::ReceiveMessage(Message const &aMessage)
{
}

/**
 * @brief Gather additional data from deserialized file.
 * @param aParser
 */
void MenuImage::ParseAdditionalData(Parser &aParser)
{
  if(aParser.Find("Surface"))
  {
#ifdef SHADER_COMPATIBLE
    PCShaderSurface *surface = (PCShaderSurface*)mObject->GET<Surface>();
#else
    Surface *surface = mObject->GET<Surface>();
#endif
    surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
    surface->Deserialize(aParser.Find("Surface"));
  }
}
