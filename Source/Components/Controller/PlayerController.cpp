//
//  PlayerController.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/25/12.
//
//

#include "PlayerController.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "GameApp.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "GraphicsManager.h"
#include "Menu.h"
#include "InputMessage.h"

PlayerController::PlayerController() : mObj(NULL)
{
}

PlayerController::~PlayerController()
{
  mObj = NULL;
}

void PlayerController::SetTarget(GameObject *aTarget)
{
  mObj = aTarget;
}

void PlayerController::DoAction(std::string const &aAction, Vector3 const &aLocation)
{
  const float moveSpeed = 200000.0f;
  if(aAction == "MoveUp")
  {
    mObj->GET<PhysicsObject>()->AddForce(Vector3(0.0f, -moveSpeed, 0.0f));
  }
  else if(aAction == "MoveDown")
  {
    mObj->GET<PhysicsObject>()->AddForce(Vector3(0.0f, moveSpeed, 0.0f));
  }
  else if(aAction == "MoveLeft")
  {
    mObj->GET<PhysicsObject>()->AddForce(Vector3(-moveSpeed, 0.0f, 0.0f));
  }
  else if(aAction == "MoveRight")
  {
    mObj->GET<PhysicsObject>()->AddForce(Vector3(moveSpeed, 0.0f, 0.0f));
  }
  else if(aAction == "QPressed")
  {
    /*GameObject *obj = GetOwner()->GetOwner()->GetOwningApp()->GET<ObjectManager>()->CreateObject(Common::RelativePath("BasicObject2.txt"));
    Vector3 pos = GetOwner()->GET<Transform>()->GetPosition();
    obj->GET<Transform>()->SetPosition(pos);*/

    //Menu *menu = new Menu(GetOwner()->GetOwner()->GetOwningApp()->GET<LevelManager>()->GetCurrentLevel(), RelativePath("BasicMenu.txt"));
  }
  else if(aAction == "Mouse")
  {
    GameObject *obj = GetOwner()->GetManager()->GetOwningApp()->GET<ObjectManager>()->CreateObject(Common::RelativePath("BasicObject2.txt"));
    Vector3 pos = GetOwner()->GetManager()->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().GetPosition();
    Vector3 size = GetOwner()->GetManager()->GetOwningApp()->GET<GraphicsManager>()->GetScreen()->GetView().GetSize();
    pos -= size/2;
    pos += aLocation;
    obj->GET<Transform>()->SetPosition(pos);
  }
}

void PlayerController::Update()
{
  
}

void PlayerController::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

void PlayerController::ReceiveMessage(Message const &aMessage)
{
  InputMessage *message = (InputMessage*)&aMessage;
  if(aMessage.GetDescription() == "Input")
    DoAction(message->GetContent(), message->GetLocation());
}
