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
      transform->SetPosition(Vector3(Common::StringToInt(aParser.Find("Transform", "PositionX")),
                                     Common::StringToInt(aParser.Find("Transform", "PositionY")),
                                     Common::StringToInt(aParser.Find("Transform", "PositionZ"))));
      transform->SetScale(Vector3(Common::StringToInt(aParser.Find("Transform", "ScaleX")),
                                  Common::StringToInt(aParser.Find("Transform", "ScaleY")),
                                  Common::StringToInt(aParser.Find("Transform", "ScaleZ"))));
      transform->SetSize(Vector3(Common::StringToInt(aParser.Find("Transform", "SizeX")),
                                 Common::StringToInt(aParser.Find("Transform", "SizeY")),
                                 Common::StringToInt(aParser.Find("Transform", "SizeZ"))));
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
      bool animated = false;
      int numAnimations = 1;
      std::vector<int> numFrames;
      numFrames.push_back(1);
      
      if(aParser.Find("Surface", "AnimationCount") != "BadString")
      {
        numFrames.clear();
        numAnimations = Common::StringToFloat(aParser.Find("Surface", "AnimationCount"));
        numFrames = Common::StringToIntVector(aParser.Find("Surface", "FrameNumbers"));
        
        std::string isAnimated = aParser.Find("Surface", "Animated");
        if(isAnimated == "true")
          animated = true;
      }
      
      surface->SetTextureCoordinateData(numAnimations, numFrames);
      surface->SetAnimated(animated);
      mObject->AddComponent(surface);
    }
  }
};

#endif /* defined(__JFramework__MenuElement__) */
