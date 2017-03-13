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
#include "Components/ChemistryElement.h"
#include "Components/ChemistryMaterial.h"
#include "Core/GameObject.h"
#include "Managers/SoundManager.h"
#include "Managers/InputManager.h"
#include "Managers/ControllerManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/PhysicsWorld.h"
#include "Core/MathExt.h"
#include "Logic/Level.h"
#include "Core/Interpolation.h"
#include "Core/QuadraticInterpolation.h"
#include "Messages/CollisionMessage.h"

#ifdef PC
#include "Graphics/PCSurface.h"
#endif

#define LUAFILECHECK()  if(mScripts.find(aFilename.ToHash()) == mScripts.end()) \
                        { \
                          std::ifstream file(Common::RelativePath("Game", aFilename.ToString()).c_str()); \
                          if(file.is_open()) \
                          { \
                            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); \
                            HashString convertedContents(contents); \
                            mScripts.insert(ScriptPair(aFilename.ToHash(), convertedContents)); \
                          } \
                          else \
                          { \
                            mScripts.insert(ScriptPair(aFilename.ToHash(), "")); \
                          } \
                        }

namespace LUABind
{
  // Our manager and scripts
  std::map<int,HashString> mScripts;
  typedef std::pair<int,HashString> ScriptPair;
  
  GameApp* StaticGameApp::mApp = NULL; 
  
  void Initialize()
  {
  }

  void RegisterClasses()
  {
    // Math
    Vector3::SerializeLUA();
    Vector4::SerializeLUA();
    Matrix33::SerializeLUA();
    
    // Messages
    CollisionMessage::SerializeLUA();

    // HashString
    HashString::SerializeLUA();
    
    // Interpolation
    Interpolation<Vector3>::SerializeLUA();
    Interpolation<Vector4>::SerializeLUA();
    QuadraticInterpolation<Vector3>::SerializeLUA();
    QuadraticInterpolation<Vector4>::SerializeLUA();

    // Transform
    Transform::SerializeLUA();
    
    // Surface
    Surface::SerializeLUA();
    PCSurface::SerializeLUA();
    
    // Physics
    PhysicsObject::SerializeLUA();
    
    // Chemistry
    ChemistryMaterial::SerializeLUA();
    ChemistryElement::SerializeLUA();

    // GameObject
    GameObject::SerializeLUA();

    // Level
    Level::SerializeLUA();

    // Managers
    Manager::SerializeLUA();
    LevelManager::SerializeLUA();
    ObjectManager::SerializeLUA();
    SoundManager::SerializeLUA();
    ControllerManager::SerializeLUA();
    InputManager::SerializeLUA();
    GraphicsManager::SerializeLUA();
    PhysicsWorld::SerializeLUA();
    
    // GameApp
    GameApp::SerializeLUA();
    
    // StaticGameApp
    SLB::Class<StaticGameApp, SLB::Instance::NoCopyNoDestroy>("StaticGameApp")
        .set("GetApp", StaticGameApp::GetApp);

    // Platform specific scripts
#ifdef PC
#endif
  }

  void LoadScriptFromFile(HashString const &aFilename)
  {
    SLB::Script script;
    LUAFILECHECK()

    script.doString(mScripts.find(aFilename.ToHash())->second.ToCharArray());
  }

  void LoadScriptFromString(HashString const &aString)
  {
    SLB::Script script;
    script.doString(aString.ToCharArray());
  }
  
  HashString const& GetScript(HashString const &aFilename)
  {
    LUAFILECHECK()
    return mScripts.find(aFilename.ToHash())->second;
  }
};
