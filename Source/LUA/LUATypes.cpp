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
#include "Components/StateObject.h"
#include "Components/CustomScript.h"
#include "Components/Camera.h"
#include "Components/FollowComponent.h"
#include "Core/GameObject.h"
#include "Managers/SoundManager.h"
#include "Managers/InputManager.h"
#include "Managers/ControllerManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/PhysicsWorld.h"
#include "Core/MathExt.h"
#include "Logic/Level.h"
#include "Core/Interpolations/Interpolation.h"
#include "Core/Interpolations/QuadraticInterpolation.h"
#include "Core/Interpolations/RoundAboutInterpolation.h"
#include "Core/Interpolations/RelativeLinearInterpolation.h"
#include "Core/Interpolations/RelativeRoundAboutInterpolation.h"
#include "Messages/CollisionMessage.h"
#include "Logic/Constants.h"
#include "Logic/SystemProperties.h"

#ifdef PC
#include "Graphics/Surfaces/PCShaderSurface.h"
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
  std::unordered_map<int,HashString> mScripts;
  typedef std::pair<int,HashString> ScriptPair;
  std::unordered_map<int, HashString> mInvalidFunctionsCache;
  
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
    RoundAboutInterpolation<Vector3>::SerializeLUA();
    RoundAboutInterpolation<Vector4>::SerializeLUA();
    RelativeLinearInterpolation<Vector3>::SerializeLUA();
    RelativeLinearInterpolation<Vector4>::SerializeLUA();
    RelativeRoundAboutInterpolation<Vector3>::SerializeLUA();
    RelativeRoundAboutInterpolation<Vector4>::SerializeLUA();

    // Transform
    Transform::SerializeLUA();
    
    // Graphics
    Surface::SerializeLUA();
    PCShaderSurface::SerializeLUA();
    Camera::SerializeLUA();
    
    // Physics
    PhysicsObject::SerializeLUA();
    
    // Chemistry
    ChemistryMaterial::SerializeLUA();
    ChemistryElement::SerializeLUA();
    
    // States
    StateObject::SerializeLUA();
    
    // CustomScript
    CustomScript::SerializeLUA();
    
    // FollowComponent
    FollowComponent::SerializeLUA();

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
    
    // Constants
    Constants::SerializeLUA();
    
    // SystemProperties
    SystemProperties::SerializeLUA();
    
    // GameApp
    GameApp::SerializeLUA();
    
    // StaticGameApp
    SLB::Class<StaticGameApp, SLB::Instance::NoCopyNoDestroy>("StaticGameApp")
        .set("GetApp", StaticGameApp::GetApp);
        
    // Helpers
    SERIALIZE_VECTOR(int, "IntVector");
    SERIALIZE_VECTOR(float, "FloatVector");
    SERIALIZE_IMMUTABLE_VECTOR(bool, "BoolVector");
    SERIALIZE_VECTOR(HashString, "HashStringVector");

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
  
  bool FunctionCaller::LoadFile(HashString const &aFilename, HashString const &aFunctionName)
  {
    HashString functionKey = aFilename + aFunctionName;
    if(mInvalidFunctionsCache.find(functionKey.ToHash()) != mInvalidFunctionsCache.end())
      return false;
    
    // Load the file up
    HashString const& contents = GetScript(aFilename);
    
    if(contents.Empty())
    {
      DebugLogPrint("LUA File: %s not found!\n", aFilename.ToCharArray());
      mInvalidFunctionsCache[functionKey.ToHash()] = functionKey;
      return false;
    }
    else if(!contents.Find(aFunctionName))
    {
      DebugLogPrint("LUA function: %s in file: %s not found!\n",
                    aFunctionName.ToCharArray(), aFilename.ToCharArray());
      mInvalidFunctionsCache[functionKey.ToHash()] = functionKey;
      return false;
    }
    
    // Try catch in case anything happens
    doString(contents.ToCharArray());
    return true;
  }
};
