//
//  MenuImage.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 3/26/13.
//
//

#include "MenuImage.h"
#include "LUATypes.h"

MenuImage::MenuImage(std::string const &aFilename) : MenuElement(aFilename)
{
  TextParser parser(Common::RelativePath("Menus", aFilename));
  ParseFile(parser);
}

MenuImage::~MenuImage()
{
}

void MenuImage::Draw()
{
}

void MenuImage::SendMessage(Message const &aMessage)
{
}

void MenuImage::ReceiveMessage(Message const &aMessage)
{
}

void MenuImage::ParseAdditionalData(Parser &aParser)
{
  // Borrowing the static game app from our LUA stuff
  GameApp* app = LUABind::StaticGameApp::GetApp();
  if(aParser.Find("Surface"))
  {
#if !defined(ANDROID) && !defined(IOS)
    PCSurface *surface = (PCSurface*)app->GET<GraphicsManager>()->CreateUISurface();
#else
    Surface *surface = new Surface();
#endif
    surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
    surface->Deserialize(aParser);
    mObject->AddComponent(surface);
  }
}
