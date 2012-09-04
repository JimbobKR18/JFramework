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
#include "LUAIncludes.h"

class LUAObject
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
};

#endif /* defined(__JFramework__LUAObject__) */
