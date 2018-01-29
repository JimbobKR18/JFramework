/*
 * MenuText.h
 *
 *  Created on: Dec 8, 2013
 *      Author: jimmy
 */

#ifndef MENUTEXT_H_
#define MENUTEXT_H_

#include "MenuElement.h"

class MenuText : public MenuElement
{
private:
  HashString      mText;
  TextRenderStyle mTextRenderStyle;

public:
  MenuText(Menu *aOwner, HashString const &aSettingsFilename, HashString const &aText, bool const aReplaceable);
  virtual ~MenuText();

  virtual void Draw();
  virtual void Update();
  virtual void SendMessage(Message const &aMessage);
  virtual void ReceiveMessage(Message const &aMessage);
  virtual void ParseAdditionalData(Parser *aParser);
};

#endif /* MENUTEXT_H_ */
