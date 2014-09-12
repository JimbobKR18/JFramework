/*
 * MenuText.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: jimmy
 */

#include "MenuText.h"
#include "LUATypes.h"
#include "Transform.h"

// TODO platform specifics

MenuText::MenuText(std::string const& aFilename) : MenuElement(aFilename)
{
  TextParser parser(Common::RelativePath("Menus", aFilename));
  ParseFile(parser);
}

MenuText::MenuText(std::string const &aSettingsFilename, std::string const &aText) : MenuElement(aSettingsFilename), mText(aText)
{
  TextParser parser(Common::RelativePath("Menus", aSettingsFilename));
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
    mFont = aParser.Find("Font", "Name")->GetValue().ToString();
    mSize = Common::StringToInt(aParser.Find("Font", "Size")->GetValue());
  }
  if(aParser.Find("ForegroundColor"))
  {
    mForegroundColor.x = aParser.Find("ForegroundColor", "r")->GetValue().ToInt();
    mForegroundColor.y = aParser.Find("ForegroundColor", "g")->GetValue().ToInt();
    mForegroundColor.z = aParser.Find("ForegroundColor", "b")->GetValue().ToInt();
    mForegroundColor.w = aParser.Find("ForegroundColor", "a")->GetValue().ToInt();
  }
  if(aParser.Find("BackgroundColor"))
  {
    mBackgroundColor.x = aParser.Find("BackgroundColor", "r")->GetValue().ToInt();
    mBackgroundColor.y = aParser.Find("BackgroundColor", "g")->GetValue().ToInt();
    mBackgroundColor.z = aParser.Find("BackgroundColor", "b")->GetValue().ToInt();
    mBackgroundColor.w = aParser.Find("BackgroundColor", "a")->GetValue().ToInt();
  }
  if(aParser.Find("Text"))
  {
    mText = aParser.Find("Text", "Value")->GetValue().ToString();
  }

#if !defined(ANDROID) && !defined(IOS)
  PCSurface *surface = (PCSurface*)app->GET<GraphicsManager>()->CreateUISurface();
  Vector3 size = surface->LoadText(mFont, mText, mForegroundColor, mBackgroundColor, mSize);
#else
  Surface *surface = new Surface();
#endif
  surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
  surface->Deserialize(aParser);
  mObject->AddComponent(surface);

  // Update texture to be the right size.
  mObject->GET<Transform>()->SetSize(size);
}
