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
    static GameApp *mApp;
    
    StaticGameApp() { mApp = NULL; }
    ~StaticGameApp() { mApp = NULL; }
    
    static GameApp *GetApp() { return mApp; }
  };
  
  void Initialize();
  void RegisterClasses();
  void LoadScriptFromFile(std::string const &aFilename);
  void LoadScriptFromString(std::string const &aString);
  std::string GetScript(std::string const &aFilename);
  
  struct FunctionCaller : public SLB::Script
  {
    template<typename T>
    void LoadFunction1p(std::string const &aFilename, std::string const &aFunctionName, T param)
    {
      // Load the file up
      doString(GetScript(aFilename).c_str());
      
      // Get the state and call the function
      lua_State *state = getState();
      SLB::LuaCall<void(T)> call(state, aFunctionName.c_str());
      call(param);
    }
  };
  
  /*void LoadFunction(std::string const &aFilename, std::string const &aFunctionName)
  {
    FunctionCaller caller;
    caller.LoadFunction1p<std::string>(aFilename, aFunctionName, "");
  }*/
  
  template<typename T>
  void LoadFunction(std::string const &aFilename, std::string const &aFunctionName, T param)
  {
    FunctionCaller caller;
    caller.LoadFunction1p<T>(aFilename, aFunctionName, param);
  }
};

#endif /* LUATYPES_H_ */
