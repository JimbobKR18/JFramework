//
//  MenuImage.h
//  JFramework
//
//  Created by Jimmy Spencer on 3/26/13.
//
//

#ifndef __JFramework__MenuImage__
#define __JFramework__MenuImage__

#include "MenuElement.h"

class MenuImage : public MenuElement
{
public:
  MenuImage(std::string const &aFilename);
  ~MenuImage();
  
  virtual void Draw();
  virtual void SendMessage(Message const &aMessage);
  virtual void ReceiveMessage(Message const &aMessage);
  virtual void ParseAdditionalData(Parser &aParser);
};

#endif /* defined(__JFramework__MenuImage__) */
