//
//  LUAObject.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/3/12.
//
//

#ifndef __JFramework__LUAObject__
#define __JFramework__LUAObject__

#include "Common.h"
#include "LuaIncludes.h"
#include "Component.h"

class LUAObject : public Component
{
private:
  lua_State *mState;
  std::string mFilename;
public:
  LUAObject();
  LUAObject(std::string const &aFilename);
  LUAObject(LUAObject const &aObject);
  virtual ~LUAObject();
  
  void SetFilename(std::string const &aFilename);
  void Execute();
  
  void CallFunction(std::string const &aFuncName);
  
  void Update();
  void SendMessage(Message const &aMessage);
  void ReceiveMessage(Message const &aMessage);
};

#endif /* defined(__JFramework__LUAObject__) */
