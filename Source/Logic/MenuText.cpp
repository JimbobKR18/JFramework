/*
 * MenuText.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: jimmy
 */

#include "MenuText.h"
#include "LUATypes.h"

// TODO platform specifics

MenuText::MenuText(std::string const& aFilename) : MenuElement(aFilename)
{
  TextParser parser(Common::RelativePath("Menus", aFilename));
  ParseFile(parser);
}

MenuText::~MenuText()
{
}

void MenuText::Draw()
{
}

void MenuText::SendMessage(Message const &aMessage)
{

}

void MenuText::ReceiveMessage(Message const &aMessage)
{

}

void MenuText::ParseAdditionalData(Parser &aParser)
{
  GameApp* app = LUABind::StaticGameApp::GetApp();
  if(aParser.Find("Font"))
  {
    mFont = aParser.Find("Font", "Name");
    mSize = Common::StringToInt(aParser.Find("Font", "Size"));
  }
  if(aParser.Find("ForegroundColor"))
  {
    mForegroundColor.x = Common::StringToInt(aParser.Find("ForegroundColor", "r"));
    mForegroundColor.y = Common::StringToInt(aParser.Find("ForegroundColor", "g"));
    mForegroundColor.z = Common::StringToInt(aParser.Find("ForegroundColor", "b"));
    mForegroundColor.w = Common::StringToInt(aParser.Find("ForegroundColor", "a"));
  }
  if(aParser.Find("ForegroundColor"))
  {
    mBackgroundColor.x = Common::StringToInt(aParser.Find("BackgroundColor", "r"));
    mBackgroundColor.y = Common::StringToInt(aParser.Find("BackgroundColor", "g"));
    mBackgroundColor.z = Common::StringToInt(aParser.Find("BackgroundColor", "b"));
    mBackgroundColor.w = Common::StringToInt(aParser.Find("BackgroundColor", "a"));
  }
  if(aParser.Find("Text"))
  {
    mText = aParser.Find("Text", "Value");
  }

#if !defined(ANDROID) && !defined(IOS)
  PCSurface *surface = (PCSurface*)app->GET<GraphicsManager>()->CreateSurface();
  surface->LoadText(mFont, mText, mForegroundColor, mBackgroundColor, mSize);
#else
  Surface *surface = new Surface();
#endif
  surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
  surface->Deserialize(aParser);
  mObject->AddComponent(surface);
}
