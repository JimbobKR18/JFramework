//
//  MenuElement.h
//  JFramework
//
//  Created by Jimmy Spencer on 3/25/13.
//
//

#ifndef __JFramework__MenuElement__
#define __JFramework__MenuElement__

#include "GameObject.h"
#include "TextParser.h"
#include "Transform.h"
#include "PCSurface.h"
#include "ObjectManager.h"
#include "GraphicsManager.h"
#include "Message.h"

class MenuElement
{
private:
  GameObject* mObject;
  int         mLayer;
  
public:
  MenuElement(std::string const &aFilename)
  {
    mObject = new GameObject(nullptr, aFilename);
    mLayer = 0;
  }
  
  virtual ~MenuElement() {}
  
  GameObject*   GetObject() { return mObject; }
  int           GetLayer() { return mLayer; }
  void          SetLayer(int const aLayer) { mLayer = aLayer; }
  
  virtual void  Draw() = 0;
  virtual void  SendMessage(Message const &aMessage) = 0;
  virtual void  ReceiveMessage(Message const &aMessage) = 0;
  
protected:
  // For now: a helper function
  void          ParseFile(Parser &aParser)
  {
    if(aParser.Find("Transform"))
    {
      // Get Position, Scale, and Size
      Transform *transform = new Transform();
      transform->Deserialize(aParser);
      mObject->AddComponent(transform);
    }
    if(aParser.Find("Surface"))
    {
#if !defined(ANDROID) && !defined(IOS)
      PCSurface *surface = new PCSurface();
      std::string imageName = aParser.Find("Surface", "TextureName");
      surface->LoadImage(imageName);
#else
      Surface *surface = new Surface();
#endif
      surface->Deserialize(aParser);
      mObject->AddComponent(surface);
    }
  }
};

#endif /* defined(__JFramework__MenuElement__) */
