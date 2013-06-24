/*
 * LUATypes.cpp
 *
 *  Created on: Dec 15, 2012
 *      Author: jimmy
 */

#include "LUATypes.h"
#include "Core/Common.h"
#include "Components/Surface.h"
#include "Components/Transform.h"
#include "Components/PhysicsObject.h"
#include "Core/GameObject.h"
#include "Core/MathExt.h"
#include "Logic/Level.h"

#ifdef PC
#include "Components/PCSurface.h"
#endif

#define LUAFILECHECK()  if(mScripts.find(aFilename) == mScripts.end()) \
                        { \
                          std::ifstream file(Common::RelativePath("Game", aFilename.c_str()).c_str()); \
                          if(file.is_open()) \
                          { \
                            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); \
                            mScripts.insert(ScriptPair(aFilename, contents)); \
                          } \
                          else \
                          { \
                            mScripts.insert(ScriptPair(aFilename, "")); \
                          } \
                        }

namespace LUABind
{
  // Our manager and scripts
  std::map<std::string,std::string> mScripts;
  typedef std::pair<std::string,std::string> ScriptPair;
  
  GameApp* StaticGameApp::mApp = NULL; 
  
  void Initialize()
  {
  }

  void RegisterClasses()
  {
    // Vector3
    Vector3::SerializeLUA();

    // Transform
    Transform::SerializeLUA();

    // GameObject
    GameObject::SerializeLUA();

    // Level
    Level::SerializeLUA();

    // Managers
    Manager::SerializeLUA();
    LevelManager::SerializeLUA();
    ObjectManager::SerializeLUA();
    // GameApp
    GameApp::SerializeLUA();
    // StaticGameApp
    SLB::Class<StaticGameApp, SLB::Instance::NoCopyNoDestroy>("StaticGameApp")
        .set("GetApp", StaticGameApp::GetApp);

    // Platform specific scripts
#ifdef PC
#endif
  }

  void LoadScriptFromFile(std::string const &aFilename)
  {
    SLB::Script script;
    LUAFILECHECK()

    script.doString(mScripts.find(aFilename)->second.c_str());
  }

  void LoadScriptFromString(std::string const &aString)
  {
    SLB::Script script;
    script.doString(aString.c_str());
  }
  
  std::string GetScript(std::string const &aFilename)
  {
    LUAFILECHECK()
    return mScripts.find(aFilename)->second;
  }
};
