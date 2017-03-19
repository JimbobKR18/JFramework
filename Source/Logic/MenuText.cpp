/*
 * MenuText.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: jimmy
 */

#if !defined(__APPLE__) && !defined(IOS) && !defined(ANDROID)
#define SHADER_COMPATIBLE
#endif

#include "MenuText.h"
#include "LUATypes.h"
#include "Transform.h"
#include "GraphicsManager.h"

#ifdef SHADER_COMPATIBLE
  #include "PCShaderSurface.h"
#elif defined(__APPLE__)
  #include "PCSurface.h"
#else
#endif

MenuText::MenuText(Menu *aOwner, HashString const &aSettingsFilename, HashString const &aText, bool const aReplaceable) :
  MenuElement(aOwner, aSettingsFilename, aReplaceable), mText(aText), mFont(), mSize(0), mMaxWidth(0), mForegroundColor(), mBackgroundColor(), mOriginalSize()
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
  if(aMessage.GetDescription() == "Finish")
  {
    Transform *transform = mObject->GET<Transform>();
    Surface *surface = mObject->GET<Surface>();
    
    transform->SetSize(mOriginalSize);
    surface->FinishAnimation();
  }
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
    mFont = aParser.Find("Font", "FontName")->GetValue().ToString();
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
  
  // Text can be overwritten in Menu file.
  if(aParser.Find("Text") && mText.Empty())
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

#ifdef SHADER_COMPATIBLE
  PCShaderSurface *surface = (PCShaderSurface*)app->GET<GraphicsManager>()->CreateUISurface();
  Vector3 size = surface->LoadText(mFont, mText, mForegroundColor, mBackgroundColor, mSize, mMaxWidth);
#elif defined(__APPLE__)
  PCSurface *surface = (PCSurface*)app->GET<GraphicsManager>()->CreateUISurface();
  Vector3 size = surface->LoadText(mFont, mText, mForegroundColor, mBackgroundColor, mSize, mMaxWidth);
#else
  Surface *surface = app->GET<GraphicsManager>()->CreateUISurface();
#endif
  surface->SetViewMode(VIEW_RELATIVE_TO_CAMERA);
  surface->Deserialize(aParser);
  
  // Update texture to be the right size.
  Transform *transform = mObject->GET<Transform>();
  mOriginalSize = size;
  transform->SetSize(size);
  
  // Reveal the text slowly rather than all at once.
  if(aParser.Find("Animation"))
  {
    Root* animation = aParser.Find("Animation");
    std::vector<std::vector<float>> animationSpeeds;
    std::vector<float> animationSpeed, speedFromFile;
    std::vector<int> numFrames;
    
    // Get speed from file.
    speedFromFile = animation->Find("AnimationSpeeds")->GetValue().ToFloatVector();
    
    // Manually set the number of frames, or auto jump a character at a time.
    if(animation->Find("NumFrames"))
    {
      numFrames = animation->Find("NumFrames")->GetValue().ToIntVector();
    }
    else
    {
      numFrames.push_back(mText.Length());
    }

    // Set frames in animation system.
    for(int i = 0; i < numFrames[0]; ++i)
    {
      if(i >= speedFromFile.size())
      {
        animationSpeed.push_back(speedFromFile[speedFromFile.size() - 1]);
      }
      else
      {
        animationSpeed.push_back(speedFromFile[i]);
      }
    }
    animationSpeeds.push_back(animationSpeed);

    surface->CreateScrollEffect(ScrollType::HORIZONTAL, mOriginalSize);
    surface->SetTextureCoordinateData(1, numFrames, animationSpeeds);
    surface->GetTextureData()->SetXGain(0, 0);
    surface->SetAnimation(0, true);
    surface->SetAnimated(true);
    transform->GetSize().x = 0;
  }
  
  mObject->AddComponent(surface);
}
