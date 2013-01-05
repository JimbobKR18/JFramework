//
//  PlayerController.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 12/25/12.
//
//

#include "PlayerController.h"
#include "PhysicsObject.h"
#include "GameApp.h"
#include "ObjectManager.h"
#include "LevelManager.h"

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

void PlayerController::DoAction(std::string const &aAction)
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
  if(aMessage.GetDescription() == "Input")
    DoAction(aMessage.GetContent());
}