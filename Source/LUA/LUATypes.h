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
  class StaticGameApp
  {
  public:
    static GameApp *mApp;
    
    StaticGameApp() { mApp = NULL; }
    ~StaticGameApp() { mApp = NULL; }
    
    static GameApp *GetApp() { return mApp; }
    static LevelManager *GetLevelManager()
    {
      return (LevelManager*)mApp->GetManager("LevelManager");
    }
    static ObjectManager *GetObjectManager()
    {
      return (ObjectManager*)mApp->GetManager("ObjectManager");
    }
    static void LoadLevel(std::string const &aFilename)
    {
      Level* level = ((LevelManager*)mApp->GetManager("LevelManager"))->CreateLevel(aFilename);
      level->Load();
    }
  };
  
  void Initialize();
  void RegisterClasses();
  void LoadScriptFromFile(std::string const &aFilename);
  void LoadScriptFromString(std::string const &aString);
};

#endif /* LUATYPES_H_ */
