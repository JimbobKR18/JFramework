#include "DefaultComponentFactory.h"
#include "GameApp.h"
#include "PhysicsWorld.h"
#include "GraphicsManager.h"
#include "ChemistryManager.h"
#include "ControllerManager.h"
#include "PhysicsObject.h"
#include "ChemistryMaterial.h"
#include "ChemistryElement.h"
#include "Transform.h"
#include "StateObject.h"
#include "CustomScript.h"
#include "SoundEmitter.h"
#include "SoundListener.h"
#include "FollowComponent.h"

#if !defined(ANDROID) && !defined(IOS)
  #include "PCShaderSurface.h"
#endif

DefaultComponentFactory::DefaultComponentFactory()
{
}

DefaultComponentFactory::~DefaultComponentFactory()
{
}

/**
 * @brief Create component, add to object, and deserialize.
 * @param aApp Game app.
 * @param aObject Object to add component to.
 * @param aNode Node that contains component info.
 * @return New component.
 */
Component* DefaultComponentFactory::CreateComponent(GameApp* aApp, GameObject *aObject, ParserNode *aNode)
{
  Component *ret = nullptr;
  if(aNode->GetName() == "PhysicsObject")
  {
    ret = aObject->HAS<PhysicsObject>() ? aObject->GET<PhysicsObject>() : aApp->GET<PhysicsWorld>()->CreateObject();
  }
  else if(aNode->GetName() == "ChemistryMaterial")
  {
    ret = aObject->HAS<ChemistryMaterial>() ? aObject->GET<ChemistryMaterial>() : aApp->GET<ChemistryManager>()->CreateMaterial(aNode->Find("Name")->GetValue());
  }
  else if(aNode->GetName() == "ChemistryElement")
  {
    ret = aObject->HAS<ChemistryElement>() ? aObject->GET<ChemistryElement>() : aApp->GET<ChemistryManager>()->CreateElement(aNode->Find("Name")->GetValue());
  }
  else if(aNode->GetName() == "Transform")
  {
    ret = aObject->HAS<Transform>() ? aObject->GET<Transform>() : new Transform();
  }
  else if(aNode->GetName() == "StateObject")
  {
    ret = aObject->HAS<StateObject>() ? aObject->GET<StateObject>() : new StateObject();
  }
  else if(aNode->GetName() == "SoundEmitter")
  {
    ret = aObject->HAS<SoundEmitter>() ? aObject->GET<SoundEmitter>() : new SoundEmitter();
  }
  else if(aNode->GetName() == "SoundListener")
  {
    ret = aObject->HAS<SoundListener>() ? aObject->GET<SoundListener>() : new SoundListener();
  }
  else if(aNode->GetName() == "Surface" || aNode->GetName() == "Renderable")
  {
    ret = aObject->GET<Renderable>();
    if(!ret)
    {
      if(aNode->Find("UIElement") && aNode->Find("UIElement")->GetValue().ToBool())
      {
#if !defined(ANDROID) && !defined(IOS)
        ret = (PCShaderSurface*)aApp->GET<GraphicsManager>()->CreateUIRenderable(aNode->GetName());
#else
        ret = aApp->GET<GraphicsManager>()->CreateUIRenderable(aNode->GetName());
#endif
      }
      else
      {
#if !defined(ANDROID) && !defined(IOS)
        ret = (PCShaderSurface*)aApp->GET<GraphicsManager>()->CreateRenderable(aNode->GetName());
#else
        ret = aApp->GET<GraphicsManager>()->CreateRenderable(aNode->GetName());
#endif
      }
    }
  }
  else if(aNode->GetName() == "Text")
  {
    ret = aObject->GET<Renderable>();
    if(!ret)
    {
      if(aNode->Find("UIElement") && aNode->Find("UIElement")->GetValue().ToBool())
      {
#if !defined(ANDROID) && !defined(IOS)
        ret = (PCShaderSurface*)aApp->GET<GraphicsManager>()->CreateUIRenderable(aNode->GetName());
#else
        ret = aApp->GET<GraphicsManager>()->CreateUIRenderable(aNode->GetName());
#endif
      }
      else
      {
#if !defined(ANDROID) && !defined(IOS)
        ret = (PCShaderSurface*)aApp->GET<GraphicsManager>()->CreateRenderable(aNode->GetName());
#else
        ret = aApp->GET<GraphicsManager>()->CreateRenderable(aNode->GetName());
#endif
      }
    }
  }
  else if(aNode->GetName() == "Camera")
  {
    ret = aObject->HAS<Camera>() ? aObject->GET<Camera>() : aApp->GET<GraphicsManager>()->CreateCamera();
  }
  else if(aNode->GetName() == "FollowComponent")
  {
    ret = aObject->HAS<FollowComponent>() ? aObject->GET<FollowComponent>() : new FollowComponent();
  }
  else if(aNode->GetName() == "CustomScript")
  {
    ret = aObject->HAS<CustomScript>() ? aObject->GET<CustomScript>() : new CustomScript();
  }
  
  if(ret != nullptr)
  {
    aObject->AddComponent(ret);
    ret->Deserialize(aNode);
  }
  else
  {
    DebugLogPrint("Unknown component %s passed in ComponentFactory\n", aNode->GetName().ToCharArray());
  }
  return ret;
}

/**
 * @brief Delete a component.
 * @param aApp Game app.
 * @param aObject Object to delete component from.
 * @param aComponent
 */
void DefaultComponentFactory::DeleteComponent(GameApp* aApp, GameObject *aObject, Component *aComponent)
{
  if(aComponent->GetDefinedUID() == PhysicsObject::GetUID())
  {
    aApp->GET<PhysicsWorld>()->RemoveObject(dynamic_cast<PhysicsObject*>(aComponent));
  }
  else if(aComponent->GetDefinedUID() == ChemistryMaterial::GetUID())
  {
    aApp->GET<ChemistryManager>()->RemoveMaterial(dynamic_cast<ChemistryMaterial*>(aComponent));
  }
  else if(aComponent->GetDefinedUID() == ChemistryElement::GetUID())
  {
    aApp->GET<ChemistryManager>()->RemoveElement(dynamic_cast<ChemistryElement*>(aComponent));
  }
  else if(aComponent->GetDefinedUID() == Renderable::GetUID())
  {
    aApp->GET<GraphicsManager>()->RemoveRenderable(dynamic_cast<Renderable*>(aComponent));
  }
  else if(aComponent->GetDefinedUID() == Camera::GetUID())
  {
    aApp->GET<GraphicsManager>()->RemoveCamera(dynamic_cast<Camera*>(aComponent));
  }
  else
  {
    aApp->GET<ControllerManager>()->RemoveController(dynamic_cast<Controller*>(aComponent));
  }
  
  //aObject->RemoveComponent(aComponent);
  delete aComponent;
}
