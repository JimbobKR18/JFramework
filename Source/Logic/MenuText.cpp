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

/**
 * @brief Does nothing.
 */
void MenuText::Draw()
{
}

/**
 * @brief Updates image size and texture corrdinates.
 */
void MenuText::Update()
{
  Transform *transform = mObject->GET<Transform>();
  Surface *surface = mObject->GET<Surface>();
  surface->Update();
  transform->GetSize().x = surface->GetTextureData()->GetXValue(1) * mOriginalSize.x;
}

/**
 * @brief Does nothing.
 * @param aMessage
 */
void MenuText::SendMessage(Message const &aMessage)
{

}

/**
 * @brief Does nothing.
 * @param aMessage
 */
void MenuText::ReceiveMessage(Message const &aMessage)
{

}

/**
 * @brief Gathers additional data from deserialized file.
 * @param aParser
 */
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
  
  // Set a max width, otherwise default to screen width..
  if(aParser.Find("MaxWidth"))
  {
    mMaxWidth = aParser.Find("MaxWidth", "Value")->GetValue().ToInt();
  }
  else
  {
    mMaxWidth = app->GET<GraphicsManager>()->GetScreen()->GetWidth();
  }

#if !defined(ANDROID) && !defined(IOS)
  PCSurface *surface = (PCSurface*)app->GET<GraphicsManager>()->CreateUISurface();
  Vector3 size = surface->LoadText(mFont, mText, mForegroundColor, mBackgroundColor, mSize, mMaxWidth);
#else
  Surface *surface = new Surface();
#endif
  surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
  surface->Deserialize(aParser);
  
  // Reveal the text slowly rather than all at once.
  if(aParser.Find("Animation"))
  {
    Root* animation = aParser.Find("Animation");
    std::vector<int> numFrames = animation->Find("NumFrames")->GetValue().ToIntVector();
    float animationSpeed = animation->Find("AnimationSpeed")->GetValue().ToFloat();

    surface->SetTextureCoordinateData(1, numFrames, animationSpeed);
    surface->GetTextureData()->SetXGain(0, 0);
    surface->SetAnimation(0, true);
    surface->SetAnimated(true);
  }
  
  mObject->AddComponent(surface);

  // Update texture to be the right size.
  mObject->GET<Transform>()->SetSize(size);
  mOriginalSize = size;
}
