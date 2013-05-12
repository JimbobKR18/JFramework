//
//  MenuImage.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 3/26/13.
//
//

#include "MenuImage.h"

MenuImage::MenuImage(std::string const &aFilename) : MenuElement(aFilename)
{
  TextParser parser(Common::RelativePath(aFilename));
  ParseFile(parser);
}

MenuImage::~MenuImage()
{
}

void MenuImage::Draw()
{
  
}
void MenuImage::SendMessage(Message const &aMessage)
{
  
}
void MenuImage::ReceiveMessage(Message const &aMessage)
{
  
}