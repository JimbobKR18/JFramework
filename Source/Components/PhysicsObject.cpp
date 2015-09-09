//
//  PhysicsObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "PhysicsObject.h"
#include "Transform.h"
#include "GameApp.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "LUATypes.h"
#include "CollisionMessage.h"
#include "PhysicsWorld.h"
#include "ObjectManager.h"

int const PhysicsObject::sUID = Common::StringHashFunction("PhysicsObject");

PhysicsObject::PhysicsObject(PhysicsWorld *aWorld) : Component(PhysicsObject::sUID), mWorld(aWorld),
                  mVelocity(0,0,0), mAcceleration(0,0,0), mForces(0,0,0),
                  mBroadSize(0,0,0), mMass(0), mInverseMass(0), 
                  mDamping(0.01f), mStatic(false), mGravity(true), mPassable(false)
{
}

PhysicsObject::~PhysicsObject()
{
  // Delete all shapes associated with this object
  for(shapeIT it = mShapes.begin(); it != mShapes.end(); ++it)
  {
    delete *it;
  }
  mShapes.clear();
  
  mWorld->RemoveObject(this);
}

/**
 * @brief Update loop
 */
void PhysicsObject::Update()
{
  // Store position for later
  Transform* transform = GetOwner()->GET<Transform>();
  Vector3 position = transform->GetPosition();

  float dt = mWorld->GetOwningApp()->GetDT();

  position += mVelocity * dt;

  // Get acceleration, adjust velocity
  Vector3 finalAcceleration = mAcceleration;
  finalAcceleration += mForces * mInverseMass;
  mVelocity += finalAcceleration * dt;
  mVelocity *= powf(mDamping, dt);

  // Set position
  transform->SetPosition(position);

  // Nullify all forces
  mForces *= 0;

  // Update the size for broadphasing
  mBroadSize = transform->GetSize() * 1.5f;
}

/**
 * @brief Send message to other objects.
 * @param aMessage Message to send.
 */
void PhysicsObject::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive message from other objects or managers.
 * @param aMessage Message to receive.
 */
void PhysicsObject::ReceiveMessage(Message const &aMessage)
{
  if(aMessage.GetDescription() != "Collision")
    return;

  CollisionMessage *message = (CollisionMessage*)&aMessage;
  GameObject *otherBody = message->GetObject(0) != GetOwner() ?
                          message->GetObject(0) : message->GetObject(1);
  std::string objectName = GetOwner()->GetFileName().substr(0, GetOwner()->GetFileName().length() - 4);
  
  if(!LUABind::LoadFunction<GameObject*>("CollisionMessages.LUA",
                                     objectName + "_CollisionReceive",
                                     otherBody))
  {
    // Do some fallback logic here
    otherBody->Interact(GetOwner());
  }
}

/**
 * @brief Serialize to file
 * @param aParser The file to serialize to.
 */
void PhysicsObject::Serialize(Parser &aParser)
{
  std::string objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  Root* object = aParser.Find(objectName);

  object->Place(objectName, "PhysicsObject", "");
  object->Place("PhysicsObject", "Gravity", Common::BoolToString(IsAffectedByGravity()));
  object->Place("PhysicsObject", "Static", Common::BoolToString(mStatic));
  object->Place("PhysicsObject", "Passable", Common::BoolToString(mPassable));
  object->Place("PhysicsObject", "Mass", Common::FloatToString(mMass));
  object->Place("PhysicsObject", "Damping", Common::FloatToString(mDamping));
  
  // Serialize each shape
  // NOTE: ALL SHAPE POSITIONS ARE IN LOCAL SPACE
  HashString const SHAPE = "Shape_";
  int curIndex = 0;
  for(shapeIT it = mShapes.begin(); it != mShapes.end(); ++it, ++curIndex)
  {
    HashString curShape = SHAPE + Common::IntToString(curIndex);
    object->Place("PhysicsObject", curShape, "");
    Root* physicsObject = object->Find("PhysicsObject");
    physicsObject->Place(curShape, "PositionX", Common::FloatToString((*it)->position.x));
    physicsObject->Place(curShape, "PositionY", Common::FloatToString((*it)->position.y));
    physicsObject->Place(curShape, "PositionZ", Common::FloatToString((*it)->position.z));
    physicsObject->Place(curShape, "Passable", Common::BoolToString((*it)->passable));
    
    switch((*it)->shape)
    {
    case Shape::CUBE:
      physicsObject->Place(curShape, "Type", "CUBE");
      physicsObject->Place(curShape, "SizeX", Common::IntToString((*it)->GetSize(0)));
      physicsObject->Place(curShape, "SizeY", Common::IntToString((*it)->GetSize(1)));
      physicsObject->Place(curShape, "SizeZ", Common::IntToString((*it)->GetSize(2)));
      break;
    case Shape::SPHERE:
      physicsObject->Place(curShape, "Type", "SPHERE");
      physicsObject->Place(curShape, "Radius", Common::IntToString((*it)->GetSize(0)));
      break;
    case Shape::TRIANGLE:
    {
      Triangle* triangle = (Triangle*)(*it);
      physicsObject->Place(curShape, "Type", "TRIANGLE");
      HashString point = "Point_";
      for(int i = 0; i < 3; ++i)
      {
        HashString pointId = point + Common::IntToString(i);
        physicsObject->Place(curShape, pointId + "X", Common::IntToString(triangle->points[i].x));
        physicsObject->Place(curShape, pointId + "Y", Common::IntToString(triangle->points[i].y));
        physicsObject->Place(curShape, pointId + "Z", Common::IntToString(triangle->points[i].z));
      }
      break;
    }
    default:
      break;
    }
  }
}

/**
 * @brief Read from file
 * @param aParser File to read from.
 */
void PhysicsObject::Deserialize(Parser &aParser)
{
  // Is our object affected by gravity?
  // What is the object's mass? Is it static?
  bool gravity = aParser.Find("PhysicsObject", "Gravity")->GetValue().ToBool();
  bool isStatic = aParser.Find("PhysicsObject", "Static")->GetValue().ToBool();
  bool isPassable = aParser.Find("PhysicsObject", "Passable")->GetValue().ToBool();
  SetMass(aParser.Find("PhysicsObject", "Mass")->GetValue().ToInt());
  mDamping = aParser.Find("PhysicsObject", "Damping")->GetValue().ToFloat();

  //TODO serialize ignore list for collisions

  // default true
  if(!gravity)
  {
    GetOwner()->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(this);
    SetAffectedByGravity(false);
  }
  
  SetStatic(isStatic);
  SetPassable(isPassable);
  
  HashString const SHAPE = "Shape_";
  int curIndex = 0;
  HashString curShape = SHAPE + Common::IntToString(curIndex);
  
  // Adding shapes
  // NOTE: ALL SHAPE POSITIONS ARE IN LOCAL SPACE
  while(aParser.Find("PhysicsObject", curShape))
  {
    Root* tempShape = aParser.Find("PhysicsObject", curShape);
    Shape* newShape = nullptr;
    
    HashString type = tempShape->Find("Type")->GetValue();
    
    // Discern type and serialize accordingly
    if(type == "CUBE")
    {
      newShape = new Cube();
      Cube* cube = (Cube*)newShape;
      cube->size = Vector3(tempShape->Find("SizeX")->GetValue().ToFloat(),
                          tempShape->Find("SizeY")->GetValue().ToFloat(),
                          tempShape->Find("SizeZ")->GetValue().ToFloat());
    }
    else if(type == "SPHERE")
    {
      newShape = new Sphere();
      Sphere* sphere = (Sphere*)newShape;
      sphere->radius = tempShape->Find("Radius")->GetValue().ToFloat();
    }
    else if(type == "TRIANGLE")
    {
      newShape = new Triangle();
      Triangle* triangle = (Triangle*)newShape;
      HashString point = "Point_";
      for(int i = 0; i < 3; ++i)
      {
        HashString pointId = point + Common::IntToString(i);
        Vector3 point(tempShape->Find(pointId + "X")->GetValue().ToFloat(),
                      tempShape->Find(pointId + "Y")->GetValue().ToFloat(),
                      tempShape->Find(pointId + "Z")->GetValue().ToFloat());
        triangle->points[i] = point;
      }
    }
    else
      assert(!"Invalid shape given");
      
    newShape->position = Vector3(tempShape->Find("PositionX")->GetValue().ToFloat(),
                                 tempShape->Find("PositionY")->GetValue().ToFloat(),
                                 tempShape->Find("PositionZ")->GetValue().ToFloat());
                                 
    // If passable flag is found, use it. Default false.
    if(tempShape->Find("Passable"))
    {
      newShape->passable = tempShape->Find("Passable")->GetValue().ToBool();
    }
    AddShape(newShape);
    
    ++curIndex;
    curShape = SHAPE + Common::IntToString(curIndex);
  }
}

/**
 * @brief Add shape to list
 * @param aShape Shape to add
 */
void PhysicsObject::AddShape(Shape* aShape)
{
  mShapes.push_back(aShape);
}

/**
 * @brief Add force to list
 * @param aForce Force to add
 */
void PhysicsObject::AddForce(Vector3 const &aForce)
{
  mForces += aForce;
}

/**
 * @brief Add object to ignore list
 * @param aObjectName Name of object to ignore
 */
void PhysicsObject::AddIgnore(std::string const &aObjectName)
{
  mIgnoreList.push_back(aObjectName);
}

/**
 * @brief Check if we should ignore the object in question
 * @param aObjectName The name of the object to maybe ignore
 * @return Whether or not to ignore collision with this object
 */
bool PhysicsObject::IgnoreObject(std::string const &aObjectName)
{
  std::vector<std::string>::iterator end = mIgnoreList.end();
  for(std::vector<std::string>::iterator it = mIgnoreList.begin(); it != end; ++it)
  {
    if(aObjectName == *it)
      return true;
  }
  return false;
}

/**
 * @brief Get velocity
 * @return Current velocity
 */
Vector3 PhysicsObject::GetVelocity() const
{
  return mVelocity;
}

/**
 * @brief Get acceleration
 * @return Current acceleration
 */
Vector3 PhysicsObject::GetAcceleration() const
{
  return mAcceleration;
}

/**
 * @brief Set velocity for this object
 * @param aVel Velocity to set to
 */
void PhysicsObject::SetVelocity(Vector3 const &aVel)
{
  mVelocity = aVel;
}

/**
 * @brief Set acceleration for this object
 * @param aAccel Acceleration to set to
 */
void PhysicsObject::SetAcceleration(Vector3 const &aAccel)
{
  mAcceleration = aAccel;
}

/**
 * @brief Get mass of this object
 * @return Mass
 */
float PhysicsObject::GetMass() const
{
	return mMass;
}

/**
 * @brief Set mass of this object
 * @param aMass Mass to set to
 */
void PhysicsObject::SetMass(float aMass)
{
	mMass = aMass;
	mInverseMass = 1.0f / mMass;
}

/**
 * @brief Is this object static? (i.e. doesn't move)
 * @return If this object is static or not
 */
bool PhysicsObject::IsStatic() const
{
  return mStatic;
}

/**
 * @brief Set whether this object is static or not
 * @param aStatic Static
 */
void PhysicsObject::SetStatic(bool aStatic)
{
  mStatic = aStatic;
}

/**
 * @brief Is this object affected by gravity force?
 * @return Is it?
 */
bool PhysicsObject::IsAffectedByGravity() const
{
  return mGravity;
}

/**
 * @brief Set if this object is affected by gravity
 * @param aGravity Gravity set.
 */
void PhysicsObject::SetAffectedByGravity(bool aGravity)
{
  mGravity = aGravity;
}

/**
 * @brief Is this object passable? Can things walk through it?
 * @return Whether or not the object is passable.
 */
bool PhysicsObject::IsPassable() const
{
  return mPassable;
}

/**
 * @brief Set if obejct is passable
 * @param aPassable Passable
 */
void PhysicsObject::SetPassable(bool aPassable)
{
  mPassable = aPassable;
}

/**
 * @brief Set size for broadphasing (bigger than normal box)
 * @return Size for broadphasing
 */
Vector3 PhysicsObject::GetBroadSize() const
{
	return mBroadSize;
}

/**
 * @brief Set broadphasing box size (bigger than normal box)
 * @param aSize Size for broadphasing
 */
void PhysicsObject::SetBroadSize(Vector3 const &aSize)
{
  mBroadSize = aSize;
}

/**
 * @brief Get all shapes
 * @return Shapes belonging to this object.
 */
std::vector<Shape*>& PhysicsObject::GetShapes()
{
  return mShapes;
}
