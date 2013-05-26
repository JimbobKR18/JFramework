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
                          std::ifstream file(Common::RelativePath(aFilename.c_str()).c_str()); \
                          if(file.is_open()) \
                          {\
                            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); \
                            mScripts.insert(ScriptPair(aFilename, contents)); \
                          }\
                          else\
                          {\
                            mScripts.insert(ScriptPair(aFilename, ""));\
                          }\
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
    /* This function will be long... and painful.
       Helper functions will be great!
       ^____________________________^'''''' */
    // Vector3
    SLB::Class<Vector3>("Vector3").constructor<float,float,float>()
        .set("x", &Vector3::x)
        .set("y", &Vector3::y)
        .set("z", &Vector3::z)
        .set("length", &Vector3::length)
        .set("SetEqual", &Vector3::operator=)
        .set("IsEqual", &Vector3::operator==)
        .set("Add", &Vector3::Add)
        .set("Subtract", &Vector3::Subtract)
        .set("Dot", &Vector3::Dot)
        .set("Cross", &Vector3::Cross)
        .set("Project", &Vector3::Project)
        .set("Invert", &Vector3::Invert);
    // Transform
    SLB::Class<Transform>("Transform").constructor()
        .set("GetPosition", &Transform::GetPosition)
        .set("GetScale", &Transform::GetScale)
        .set("GetSize", &Transform::GetSize)
        .set("SetPosition", &Transform::SetPosition)
        .set("SetScale", &Transform::SetScale)
        .set("SetSize", &Transform::SetSize);
    // GameObject
    SLB::Class<GameObject>("GameObject")
        .set("GetComponent", &GameObject::GetComponent)
        .set("GetTransform", &GameObject::GET<Transform>);
    // Level
    SLB::Class<Level>("Level").constructor()
        .set("Load", &Level::Load)
        .set("Unload", &Level::Unload);
    // Managers
    SLB::Class<Manager, SLB::Instance::NoCopy>("Manager")
        .set("GetOwningApp", &Manager::GetOwningApp);
    SLB::Class<LevelManager>("LevelManager").inherits<Manager>()
        .set("CreateLevel", &LevelManager::CreateLevel)
        .set("LoadLevel", &LevelManager::LoadLevel);
    SLB::Class<ObjectManager>("ObjectManager").inherits<Manager>()
        .set("CreateObject", &ObjectManager::CreateObject);
    // GameApp
    SLB::Class<GameApp>("GameApp")
        .set("GetManager", &GameApp::GetManager)
        .set("GetLevelManager", &GameApp::GET<LevelManager>)
        .set("GetObjectManager", &GameApp::GET<ObjectManager>);
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
