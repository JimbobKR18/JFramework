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
                  mDamping(0.01f), mRestitution(0.0f), mStatic(false),
                  mGravity(true), mPassable(false)
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
  mWorld = nullptr;
}

/**
 * @brief Update loop
 */
void PhysicsObject::Update()
{
  // Store position for later
  Transform* transform = GetOwner()->GET<Transform>();
  Vector3 position = transform->GetPosition();

  float dt = mWorld->GetOwningApp()->GetAppStep();

  position += mVelocity * dt;

  // Get acceleration, adjust velocity
  Vector3 finalAcceleration = mAcceleration;
  finalAcceleration += mForces * mInverseMass;
  mVelocity += finalAcceleration * dt;
  mVelocity *= powf(mDamping, dt);

  // Set position
  transform->SetPosition(position);

  // Nullify all forces
  ClearForces();

  // Update the size for broadphasing
  mBroadSize = transform->GetSize().Multiply(transform->GetScale()) * 1.5f;
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
  HashString const objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const PHYSICS_OBJECT = "PhysicsObject";
  Root* object = aParser.Find(objectName);
  
  object->Place(objectName, PHYSICS_OBJECT, "");
  object->Place(PHYSICS_OBJECT, "Gravity", Common::BoolToString(IsAffectedByGravity()));
  object->Place(PHYSICS_OBJECT, "Static", Common::BoolToString(mStatic));
  object->Place(PHYSICS_OBJECT, "Passable", Common::BoolToString(mPassable));
  object->Place(PHYSICS_OBJECT, "Mass", Common::FloatToString(mMass));
  object->Place(PHYSICS_OBJECT, "Damping", Common::FloatToString(mDamping));
  object->Place(PHYSICS_OBJECT, "Restitution", Common::FloatToString(mRestitution));
  
  // Serialize each shape
  // NOTE: ALL SHAPE POSITIONS ARE IN LOCAL SPACE
  Root* physicsObject = object->Find("PhysicsObject");
  HashString const SHAPE = "Shape_";
  int curIndex = 0;
  for(shapeIT it = mShapes.begin(); it != mShapes.end(); ++it, ++curIndex)
  {
    HashString curShape = SHAPE + Common::IntToString((*it)->id);
    Vector3 localPosition = (*it)->position;
    physicsObject->Place(PHYSICS_OBJECT, curShape, "");
    Root* shapeObject = physicsObject->Find(curShape);
    
    shapeObject->Place(curShape, "PositionX", Common::FloatToString(localPosition.x));
    shapeObject->Place(curShape, "PositionY", Common::FloatToString(localPosition.y));
    shapeObject->Place(curShape, "PositionZ", Common::FloatToString(localPosition.z));
    shapeObject->Place(curShape, "Passable", Common::BoolToString((*it)->passable));
    
    switch((*it)->shape)
    {
    case Shape::CUBE:
      shapeObject->Place(curShape, "Type", "CUBE");
      shapeObject->Place(curShape, "SizeX", Common::IntToString((*it)->GetSize(0)));
      shapeObject->Place(curShape, "SizeY", Common::IntToString((*it)->GetSize(1)));
      shapeObject->Place(curShape, "SizeZ", Common::IntToString((*it)->GetSize(2)));
      break;
    case Shape::SPHERE:
      shapeObject->Place(curShape, "Type", "SPHERE");
      shapeObject->Place(curShape, "Radius", Common::IntToString((*it)->GetSize(0)));
      break;
    case Shape::TRIANGLE:
    {
      Triangle* triangle = (Triangle*)(*it);
      shapeObject->Place(curShape, "Type", "TRIANGLE");
      HashString point = "Point_";
      for(int i = 0; i < 3; ++i)
      {
        HashString pointId = point + Common::IntToString(i);
        shapeObject->Place(curShape, pointId + "X", Common::IntToString(triangle->points[i].x));
        shapeObject->Place(curShape, pointId + "Y", Common::IntToString(triangle->points[i].y));
        shapeObject->Place(curShape, pointId + "Z", Common::IntToString(triangle->points[i].z));
      }
      break;
    }
    case Shape::LINE:
    {
      Line *line = (Line*)(*it);
      shapeObject->Place(curShape, "Type", "LINE");
      shapeObject->Place(curShape, "Length", Common::IntToString((*it)->GetSize(0)));
      shapeObject->Place(curShape, "DirectionX", Common::IntToString(line->direction.x));
      shapeObject->Place(curShape, "DirectionY", Common::IntToString(line->direction.y));
      shapeObject->Place(curShape, "DirectionZ", Common::IntToString(line->direction.z));
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
  mRestitution = aParser.Find("PhysicsObject", "Restitution")->GetValue().ToFloat();

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
      cube->shape = Shape::CUBE;
    }
    else if(type == "SPHERE")
    {
      newShape = new Sphere();
      Sphere* sphere = (Sphere*)newShape;
      sphere->radius = tempShape->Find("Radius")->GetValue().ToFloat();
      sphere->shape = Shape::SPHERE;
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
      triangle->shape = Shape::TRIANGLE;
    }
    else if(type == "LINE")
    {
      newShape = new Line();
      Line *line = (Line*)newShape;
      line->direction = Vector3(tempShape->Find("DirectionX")->GetValue().ToFloat(),
                                tempShape->Find("DirectionY")->GetValue().ToFloat(),
                                tempShape->Find("DirectionZ")->GetValue().ToFloat());
      line->length = tempShape->Find("Length")->GetValue().ToFloat();
      line->shape = Shape::LINE;
    }
    else
      assert(!"Invalid shape given");
      
    newShape->id = curIndex;
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
 * @brief Clear all forces for object
 */
void PhysicsObject::ClearForces()
{
  mForces *= 0;
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
void PhysicsObject::SetMass(float const aMass)
{
	mMass = aMass;
	mInverseMass = 1.0f / mMass;
}

/**
 * @brief Get damping for this object
 * @return Damping
 */
float PhysicsObject::GetDamping() const
{
  return mDamping;
}

/**
 * @brief Set damping for this object
 * @param aDamping Damping to set to
 */
void PhysicsObject::SetDamping(float const aDamping)
{
  mDamping = aDamping;
}

/**
 * @brief Get restitution for this object
 * @return Restitution
 */
float PhysicsObject::GetRestitution() const
{
  return mRestitution;
}

/**
 * @brief Set restitution for this object
 * @param aRestitution Restitution to set to
 */
void PhysicsObject::SetRestiution(float const aRestitution)
{
  mRestitution = aRestitution;
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
void PhysicsObject::SetStatic(bool const aStatic)
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
void PhysicsObject::SetAffectedByGravity(bool const aGravity)
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
void PhysicsObject::SetPassable(bool const aPassable)
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
