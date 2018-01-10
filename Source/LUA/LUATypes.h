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

#define SERIALIZE_VECTOR(T, NAME) SLB::Class<std::vector<T>>(NAME) \
        .constructor() \
        .set("push_back", static_cast<void(std::vector<T>::*)(const std::vector<T>::value_type&)>(&std::vector<T>::push_back)) \
        .set("pop_back", &std::vector<T>::pop_back) \
        .set("size", &std::vector<T>::size) \
        .set("empty", &std::vector<T>::empty) \
        .set("at", static_cast<std::vector<T>::reference(std::vector<T>::*)(std::vector<T>::size_type)>(&std::vector<T>::at))
        
#define SERIALIZE_IMMUTABLE_VECTOR(T, NAME) SLB::Class<std::vector<T>>(NAME) \
        .constructor() \
        .set("size", &std::vector<T>::size) \
        .set("empty", &std::vector<T>::empty) \
        .set("at", static_cast<std::vector<T>::reference(std::vector<T>::*)(std::vector<T>::size_type)>(&std::vector<T>::at))

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
  
  void                  Initialize();
  void                  RegisterClasses();
  void                  LoadScriptFromFile(std::string const &aFilename);
  void                  LoadScriptFromString(std::string const &aString);
  HashString const&     GetScript(HashString const &aFilename);
  
  struct FunctionCaller : public SLB::Script
  {
    bool LoadFile(HashString const &aFilename, HashString const &aFunctionName);
    
    // Returns true if function called successfully
    template<typename T>
    bool LoadFunction1p(HashString const &aFilename, HashString const &aFunctionName, T param)
    {
      bool result = LoadFile(aFilename, aFunctionName);
      if(!result)
        return false;
    
      // Get the state and call the function
      lua_State *state = getState();
      SLB::LuaCall<void(T)> call(state, aFunctionName.ToCharArray());
      call(param);
      
      // Return on success
      return true;
    }
    
    template<typename T, typename U>
    bool LoadFunction2p(HashString const &aFilename, HashString const &aFunctionName, T param1, U param2)
    {
      bool result = LoadFile(aFilename, aFunctionName);
      if(!result)
        return false;
    
      // Get the state and call the function
      lua_State *state = getState();
      SLB::LuaCall<void(T,U)> call(state, aFunctionName.ToCharArray());
      call(param1, param2);
      
      // Return on success
      return true;
    }
    
    template<typename T, typename U, typename V>
    bool LoadFunction3p(HashString const &aFilename, HashString const &aFunctionName, T param1, U param2, V param3)
    {
      bool result = LoadFile(aFilename, aFunctionName);
      if(!result)
        return false;
    
      // Get the state and call the function
      lua_State *state = getState();
      SLB::LuaCall<void(T,U,V)> call(state, aFunctionName.ToCharArray());
      call(param1, param2, param3);
      
      // Return on success
      return true;
    }
    
    template<typename T, typename U, typename V, typename W>
    bool LoadFunction4p(HashString const &aFilename, HashString const &aFunctionName, T param1, U param2, V param3, W param4)
    {
      bool result = LoadFile(aFilename, aFunctionName);
      if(!result)
        return false;
    
      // Get the state and call the function
      lua_State *state = getState();
      SLB::LuaCall<void(T,U,V,W)> call(state, aFunctionName.ToCharArray());
      call(param1, param2, param3, param4);
      
      // Return on success
      return true;
    }
  };
  
  template<typename T>
  bool LoadFunction(HashString const &aFilename, HashString const &aFunctionName, T param)
  {
    FunctionCaller caller;
    return caller.LoadFunction1p<T>(aFilename, aFunctionName, param);
  }
  
  template<typename T, typename U>
  bool LoadFunction(HashString const &aFilename, HashString const &aFunctionName, T param1, U param2)
  {
    FunctionCaller caller;
    return caller.LoadFunction2p<T,U>(aFilename, aFunctionName, param1, param2);
  }
  
  template<typename T, typename U, typename V>
  bool LoadFunction(HashString const &aFilename, HashString const &aFunctionName, T param1, U param2, V param3)
  {
    FunctionCaller caller;
    return caller.LoadFunction3p<T,U,V>(aFilename, aFunctionName, param1, param2, param3);
  }
  
  template<typename T, typename U, typename V, typename W>
  bool LoadFunction(HashString const &aFilename, HashString const &aFunctionName, T param1, U param2, V param3, W param4)
  {
    FunctionCaller caller;
    return caller.LoadFunction4p<T,U,V,W>(aFilename, aFunctionName, param1, param2, param3, param4);
  }
};

#endif /* LUATYPES_H_ */
