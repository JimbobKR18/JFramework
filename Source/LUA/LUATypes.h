/*
 * LUATypes.h
 *
 *  Created on: Dec 15, 2012
 *      Author: jimmy
 */

#ifndef LUATYPES_H_
#define LUATYPES_H_

#include "Core/LuaIncludes.h"
#include "GameApp.h"
#include "Managers/Manager.h"
#include "Managers/LevelManager.h"
#include "Managers/ObjectManager.h"

namespace LUABind
{
  // A way to access the app and manage resources
  struct StaticGameApp
  {
    static GameApp* mApp;
    
    StaticGameApp() { mApp = NULL; }
    ~StaticGameApp() { mApp = NULL; }
    
    static GameApp *GetApp() { return mApp; }
  };
  
  void          Initialize();
  void          RegisterClasses();
  void          LoadScriptFromFile(std::string const &aFilename);
  void          LoadScriptFromString(std::string const &aString);
  std::string   GetScript(std::string const &aFilename);
  
  struct FunctionCaller : public SLB::Script
  {
    // Returns true if function called successfully
    template<typename T>
    bool LoadFunction1p(std::string const &aFilename, std::string const &aFunctionName, T param)
    {
      // Load the file up
      std::string contents = GetScript(aFilename);
      
      if(contents.empty())
      {
        DebugLogPrint("LUA File: %s not found!\n", aFilename.c_str());
        return false;
      }
      else if(contents.find(aFunctionName) == std::string::npos)
      {
        DebugLogPrint("LUA function: %s in file: %s not found!\n",
                      aFunctionName.c_str(), aFilename.c_str());
        return false;
      }
      
      doString(contents.c_str());
      
      // Get the state and call the function
      lua_State *state = getState();
      SLB::LuaCall<void(T)> call(state, aFunctionName.c_str());
      call(param);
      return true;
    }
  };
  
  /*void LoadFunction(std::string const &aFilename, std::string const &aFunctionName)
  {
    FunctionCaller caller;
    caller.LoadFunction1p<std::string>(aFilename, aFunctionName, "");
  }*/
  
  template<typename T>
  bool LoadFunction(std::string const &aFilename, std::string const &aFunctionName, T param)
  {
    FunctionCaller caller;
    return caller.LoadFunction1p<T>(aFilename, aFunctionName, param);
  }
};

#endif /* LUATYPES_H_ */
