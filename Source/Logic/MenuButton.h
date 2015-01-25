#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "MathExt.h"
#include "Shape.h"
#include "MenuImage.h"

class MenuButton: public MenuImage
{
private:
  Cube mClickableArea;
  
public:
  MenuButton(std::string const &aFilename);
  virtual ~MenuButton();

  virtual void Draw();
  virtual void ParseAdditionalData(Parser& aParser);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void SendMessage(Message const& aMessage);
  virtual void OnDownClick() = 0;
  virtual void OnUpClick() = 0;
};

#endif // MENUBUTTON_H
