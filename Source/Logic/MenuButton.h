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
  MenuButton(Menu *aOwner, HashString const &aFilename);
  virtual ~MenuButton();

  virtual void Draw();
  virtual void Update();
  virtual void ParseAdditionalData(Parser& aParser);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void SendMessage(Message const& aMessage);
  virtual void OnDownClick() = 0;
  virtual void OnUpClick() = 0;
};

#endif // MENUBUTTON_H
