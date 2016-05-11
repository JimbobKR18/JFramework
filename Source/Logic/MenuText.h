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
  HashString    mText;
  HashString    mFont;
  int           mSize;
  int           mMaxWidth;
  Vector4       mForegroundColor;
  Vector4       mBackgroundColor;
  Vector3       mOriginalSize;

public:
  MenuText(Menu *aOwner, HashString const &aFilename);
  MenuText(Menu *aOwner, HashString const &aSettingsFilename, HashString const &aText);
  virtual ~MenuText();

  virtual void Draw();
  virtual void Update();
  virtual void SendMessage(Message const &aMessage);
  virtual void ReceiveMessage(Message const &aMessage);
  virtual void ParseAdditionalData(Parser &aParser);
};

#endif /* MENUTEXT_H_ */
