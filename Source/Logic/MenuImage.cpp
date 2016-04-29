//
//  MenuImage.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 3/26/13.
//
//

#include "MenuImage.h"
#include "LUATypes.h"
#include "GraphicsManager.h"

MenuImage::MenuImage(Menu *aOwner, HashString const &aFilename) : MenuElement(aOwner, aFilename)
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
  // Borrowing the static game app from our LUA stuff
  GameApp* app = LUABind::StaticGameApp::GetApp();
  if(aParser.Find("Surface"))
  {
#if !defined(ANDROID) && !defined(IOS)
    PCShaderSurface *surface = (PCShaderSurface*)mObject->GET<Surface>();
#else
    Surface *surface = mObject->GET<Surface>();
#endif
    surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
  }
}
