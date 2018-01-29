/*
 * MenuText.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: jimmy
 */

#if !defined(IOS) && !defined(ANDROID)
#define SHADER_COMPATIBLE
#endif

#include "MenuText.h"
#include "LUATypes.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "SystemProperties.h"
#include "Menu.h"

#ifdef SHADER_COMPATIBLE
  #include "PCShaderSurface.h"
#else
#endif

MenuText::MenuText(Menu *aOwner, HashString const &aSettingsFilename, HashString const &aText, bool const aReplaceable) :
  MenuElement(aOwner, aSettingsFilename, aReplaceable), mText(aText), mTextRenderStyle(TextRenderStyle::DEFAULT_RENDER_STYLE)
{
  Parser* parser = ParserFactory::CreateInputParser("Menus", aSettingsFilename);
  ParseFile(parser);
  delete parser;
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
    
    transform->SetSize(surface->GetOriginalSize());
    surface->FinishAnimation();
  }
}

/**
 * @brief Gathers additional data from deserialized file.
 * @param aParser
 */
void MenuText::ParseAdditionalData(Parser *aParser)
{
  if(aParser->Find("Text")->Find("RenderStyle"))
  {
    HashString textRenderStyle = aParser->Find("Text")->Find("RenderStyle")->GetValue();
    if(textRenderStyle == "SMOOTH")
      mTextRenderStyle = TextRenderStyle::SMOOTH_RENDER_STYLE;
    else if(textRenderStyle == "CHARACTER_BY_CHARACTER")
      mTextRenderStyle = TextRenderStyle::CHARACTER_BY_CHARACTER_STYLE;
    else
    {
      DebugLogPrint("Incorrect render style %s used for text rendering.", textRenderStyle.ToCharArray());
      assert(!"Incorrect render style used for text rendering.");
    }
  }
  
  Surface *surface = mObject->GET<Surface>();
  surface->SetViewMode(Viewspace::VIEW_RELATIVE_TO_CAMERA);
  surface->LoadText(mText, mTextRenderStyle);
}
