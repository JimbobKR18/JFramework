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
  std::string   mText;
  std::string   mFont;
  int           mSize;
  int           mMaxWidth;
  Vector4       mForegroundColor;
  Vector4       mBackgroundColor;

public:
  MenuText(std::string const &aFilename);
  MenuText(std::string const &aSettingsFilename, std::string const &aText);
  virtual ~MenuText();

  virtual void Draw();
  virtual void SendMessage(Message const &aMessage);
  virtual void ReceiveMessage(Message const &aMessage);
  virtual void ParseAdditionalData(Parser &aParser);
};

#endif /* MENUTEXT_H_ */
