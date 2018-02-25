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
                  mDamping(0.01f), mRestitution(0.0f), mMaximumVelocity(-1.0f), mStatic(false),
                  mGravity(true), mPassable(false), mActive(true), mIgnoreList(), mShapes(), mJoints()
{
}

PhysicsObject::~PhysicsObject()
{
  // Delete all shapes associated with this object
  for(ShapeIT it = mShapes.begin(); it != mShapes.end(); ++it)
  {
    delete *it;
  }
  mShapes.clear();
  
  for(JointIT it = mJoints.begin(); it != mJoints.end(); ++it)
  {
    delete *it;
  }
  mJoints.clear();
  
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
  
  // Cap velocity
  if(mMaximumVelocity > 0.0f && mVelocity.length() > mMaximumVelocity)
  {
    mVelocity = mVelocity.normalize() * mMaximumVelocity;
  }

  // Set position
  transform->SetPosition(position);

  // Nullify all forces
  ClearForces();

  // Update the size for broadphasing, may be a bit slow, though
  Vector3 maxRange = transform->GetSize().Multiply(transform->GetHierarchicalScale());
  ShapeIT end = mShapes.end();
  for(ShapeIT it = mShapes.begin(); it != end; ++it)
  {
    Shape *shape = *it;
    maxRange.x = Greater<float>(maxRange.x, shape->position[0] + shape->GetSize(0) * transform->GetHierarchicalScale().x);
    maxRange.y = Greater<float>(maxRange.y, shape->position[1] + shape->GetSize(1) * transform->GetHierarchicalScale().y);
    maxRange.z = Greater<float>(maxRange.z, shape->position[2] + shape->GetSize(2) * transform->GetHierarchicalScale().z);
  }
  mBroadSize = maxRange * 1.5f;
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
  HashString objectName = GetOwner()->GetFileName().SubString(0, GetOwner()->GetFileName().Length() - 4);
  
  if(!LUABind::LoadFunction<CollisionMessage*>("CollisionMessages.lua",
                                     objectName + "_CollisionReceive",
                                     message))
  {
    // Do some fallback logic here
    otherBody->Interact(GetOwner());
  }
}

/**
 * @brief Serialize to file
 * @param aParserNode The node to serialize to.
 */
void PhysicsObject::Serialize(ParserNode *aNode)
{
  HashString const PHYSICS_OBJECT = "PhysicsObject";
  aNode->Place(PHYSICS_OBJECT, "");
  ParserNode* object = aNode->Find(PHYSICS_OBJECT);
  
  object->Place("Gravity", Common::BoolToString(IsAffectedByGravity()));
  object->Place("Static", Common::BoolToString(mStatic));
  object->Place("Passable", Common::BoolToString(mPassable));
  object->Place("Mass", Common::FloatToString(mMass));
  object->Place("Damping", Common::FloatToString(mDamping));
  object->Place("Restitution", Common::FloatToString(mRestitution));
  object->Place("MaxVelocity", Common::FloatToString(mMaximumVelocity));
  
  if(!mIgnoreList.empty())
  {
    std::vector<std::string> ignoreList;
    for(IgnoreIT it = mIgnoreList.begin(); it != mIgnoreList.end(); ++it)
    {
      ignoreList.push_back(it->second);
    }
    object->Place("IgnoreList", Common::StringVectorToString(ignoreList));
  }
  
  // Serialize each shape
  // NOTE: ALL SHAPE POSITIONS ARE IN LOCAL SPACE
  HashString const SHAPE = "Shape_";
  int curIndex = 0;
  for(ShapeIT it = mShapes.begin(); it != mShapes.end(); ++it, ++curIndex)
  {
    HashString curShape = SHAPE + Common::IntToString((*it)->id);
    Vector3 localPosition = (*it)->position;
    object->Place(curShape, "");
    ParserNode* shapeObject = object->Find(curShape);
    
    shapeObject->Place("PositionX", Common::FloatToString(localPosition.x));
    shapeObject->Place("PositionY", Common::FloatToString(localPosition.y));
    shapeObject->Place("PositionZ", Common::FloatToString(localPosition.z));
    shapeObject->Place("Passable", Common::BoolToString((*it)->passable));
    
    switch((*it)->shape)
    {
    case Shape::AABB:
      shapeObject->Place("Type", "AABB");
      shapeObject->Place("SizeX", Common::FloatToString((*it)->GetSize(0)));
      shapeObject->Place("SizeY", Common::FloatToString((*it)->GetSize(1)));
      shapeObject->Place("SizeZ", Common::FloatToString((*it)->GetSize(2)));
      break;
    case Shape::SPHERE:
      shapeObject->Place("Type", "SPHERE");
      shapeObject->Place("Radius", Common::FloatToString((*it)->GetSize(0)));
      break;
    case Shape::TRIANGLE:
    {
      Triangle* triangle = (Triangle*)(*it);
      shapeObject->Place("Type", "TRIANGLE");
      HashString point = "Point_";
      for(int i = 0; i < 3; ++i)
      {
        HashString pointId = point + Common::FloatToString(i);
        shapeObject->Place(pointId + "X", Common::FloatToString(triangle->points[i].x));
        shapeObject->Place(pointId + "Y", Common::FloatToString(triangle->points[i].y));
        shapeObject->Place(pointId + "Z", Common::FloatToString(triangle->points[i].z));
      }
      break;
    }
    case Shape::LINE:
    {
      Line *line = (Line*)(*it);
      shapeObject->Place("Type", "LINE");
      shapeObject->Place("Length", Common::FloatToString((*it)->GetSize(0)));
      shapeObject->Place("DirectionX", Common::FloatToString(line->direction.x));
      shapeObject->Place("DirectionY", Common::FloatToString(line->direction.y));
      shapeObject->Place("DirectionZ", Common::FloatToString(line->direction.z));
      break;
    }
    case Shape::OBB:
    {
      OrientedBoundingBox *obb = (OrientedBoundingBox*)(*it);
      shapeObject->Place("Type", "OBB");
      shapeObject->Place("UpX", Common::FloatToString(obb->up.x));
      shapeObject->Place("UpY", Common::FloatToString(obb->up.y));
      shapeObject->Place("UpZ", Common::FloatToString(obb->up.z));
      shapeObject->Place("RightX", Common::FloatToString(obb->right.x));
      shapeObject->Place("RightY", Common::FloatToString(obb->right.y));
      shapeObject->Place("RightZ", Common::FloatToString(obb->right.z));
      shapeObject->Place("ForwardX", Common::FloatToString(obb->forward.x));
      shapeObject->Place("ForwardY", Common::FloatToString(obb->forward.y));
      shapeObject->Place("ForwardZ", Common::FloatToString(obb->forward.z));
      shapeObject->Place("ExtentX", Common::FloatToString(obb->extents.x));
      shapeObject->Place("ExtentY", Common::FloatToString(obb->extents.y));
      shapeObject->Place("ExtentZ", Common::FloatToString(obb->extents.z));
      break;
    }
    default:
      break;
    }
  }
  
  // Joints
  HashString const JOINT = "Joint_";
  curIndex = 0;
  for(JointIT it = mJoints.begin(); it != mJoints.end(); ++it, ++curIndex)
  {
    HashString curJoint = JOINT + Common::IntToString((*it)->GetID());
    Vector3 localPosition = (*it)->GetPosition();
    object->Place(curJoint, "");
    ParserNode* jointObject = object->Find(curJoint);
    
    jointObject->Place("PositionX", Common::FloatToString(localPosition.x));
    jointObject->Place("PositionY", Common::FloatToString(localPosition.y));
    jointObject->Place("PositionZ", Common::FloatToString(localPosition.z));
  }
}

/**
 * @brief Read from file
 * @param aNode ParserNode to read from.
 */
void PhysicsObject::Deserialize(ParserNode *aNode)
{
  // Is our object affected by gravity?
  // What is the object's mass? Is it static?
  if(aNode->Find("Static"))
    SetStatic(aNode->Find("Static")->GetValue().ToBool());
  if(aNode->Find("Passable"))
    SetPassable(aNode->Find("Passable")->GetValue().ToBool());
  if(aNode->Find("Mass"))
    SetMass(aNode->Find("Mass")->GetValue().ToFloat());
  if(aNode->Find("Damping"))
    mDamping = aNode->Find("Damping")->GetValue().ToFloat();
  if(aNode->Find("Restitution"))
    mRestitution = aNode->Find("Restitution")->GetValue().ToFloat();

  if(aNode->Find("IgnoreList"))
  {
    mIgnoreList.clear();
    std::vector<std::string> ignoreList = aNode->Find("IgnoreList")->GetValue().ToStringVector();
    
    for(std::vector<std::string>::iterator it = ignoreList.begin(); it != ignoreList.end(); ++it)
    {
      mIgnoreList[Common::StringHashFunction(*it)] = *it;
    }
  }
  if(aNode->Find("MaxVelocity"))
  {
    mMaximumVelocity = aNode->Find("MaxVelocity")->GetValue().ToFloat();
  }

  // Default true
  if(aNode->Find("Gravity") && !aNode->Find("Gravity")->GetValue().ToBool())
  {
    GetOwner()->GetManager()->GetOwningApp()->GET<PhysicsWorld>()->UnregisterGravity(this);
    SetAffectedByGravity(false);
  }
  
  HashString const SHAPE = "Shape_";
  int curIndex = 0;
  HashString curShape = SHAPE + Common::IntToString(curIndex);
  
  // Adding shapes
  // NOTE: ALL SHAPE POSITIONS ARE IN LOCAL SPACE
  if(aNode->Find(curShape))
  {
    for(ShapeIT it = mShapes.begin(); it != mShapes.end(); ++it)
    {
      delete *it;
    }
    mShapes.clear();
  }
  while(aNode->Find(curShape))
  {
    ParserNode* tempShape = aNode->Find(curShape);
    Shape* newShape = nullptr;
    
    HashString type = tempShape->Find("Type")->GetValue();
    
    // Discern type and serialize accordingly
    if(type == "CUBE" || type == "AABB")
    {
      newShape = new AxisAlignedBoundingBox();
      AxisAlignedBoundingBox* cube = (AxisAlignedBoundingBox*)newShape;
      cube->size = Vector3(tempShape->Find("SizeX")->GetValue().ToFloat(),
                          tempShape->Find("SizeY")->GetValue().ToFloat(),
                          tempShape->Find("SizeZ")->GetValue().ToFloat());
      cube->shape = Shape::AABB;
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
                                tempShape->Find("DirectionZ")->GetValue().ToFloat()).normalize();
      line->length = tempShape->Find("Length")->GetValue().ToFloat();
      line->shape = Shape::LINE;
    }
    else if(type == "OBB")
    {
      newShape = new OrientedBoundingBox();
      OrientedBoundingBox *obb = (OrientedBoundingBox*)newShape;
      obb->up = Vector3(tempShape->Find("UpX")->GetValue().ToFloat(),
                        tempShape->Find("UpY")->GetValue().ToFloat(),
                        tempShape->Find("UpZ")->GetValue().ToFloat()).normalize();
      obb->right = Vector3(tempShape->Find("RightX")->GetValue().ToFloat(),
                        tempShape->Find("RightY")->GetValue().ToFloat(),
                        tempShape->Find("RightZ")->GetValue().ToFloat()).normalize();
      obb->forward = Vector3(tempShape->Find("ForwardX")->GetValue().ToFloat(),
                        tempShape->Find("ForwardY")->GetValue().ToFloat(),
                        tempShape->Find("ForwardZ")->GetValue().ToFloat()).normalize();
      obb->extents = Vector3(tempShape->Find("ExtentX")->GetValue().ToFloat(),
                        tempShape->Find("ExtentY")->GetValue().ToFloat(),
                        tempShape->Find("ExtentZ")->GetValue().ToFloat());
      obb->shape = Shape::OBB;
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
  
  HashString const JOINT = "Joint_";
  curIndex = 0;
  HashString curJoint = JOINT + Common::IntToString(curIndex);
  
  // Adding joints
  // NOTE: ALL JOINT POSITIONS ARE IN LOCAL SPACE
  if(aNode->Find(curJoint))
  {
    for(JointIT it = mJoints.begin(); it != mJoints.end(); ++it)
    {
      delete *it;
    }
    mJoints.clear();
  }
  while(aNode->Find(curJoint))
  {
    ParserNode* tempJoint = aNode->Find(curJoint);
    float positionX = tempJoint->Find("PositionX")->GetValue().ToFloat();
    float positionY = tempJoint->Find("PositionY")->GetValue().ToFloat();
    float positionZ = tempJoint->Find("PositionZ")->GetValue().ToFloat();
    Joint *joint = new Joint(curIndex, Vector3(positionX,positionY,positionZ), this);
    AddJoint(joint);
    ++curIndex;
    curJoint = JOINT + Common::IntToString(curIndex);
  }
}

/**
 * @brief Make this object usable in LUA
 */
void PhysicsObject::SerializeLUA()
{
  SLB::Class<PhysicsObject>("PhysicsObject")
    .inherits<Component>()
    .set("GetVelocity", &PhysicsObject::GetVelocity)
    .set("SetVelocity", &PhysicsObject::SetVelocity)
    .set("GetAcceleration", &PhysicsObject::GetAcceleration)
    .set("SetAcceleration", &PhysicsObject::SetAcceleration)
    .set("GetMass", &PhysicsObject::GetMass)
    .set("SetMass", &PhysicsObject::SetMass)
    .set("GetDamping", &PhysicsObject::GetDamping)
    .set("SetDamping", &PhysicsObject::SetDamping)
    .set("GetRestitution", &PhysicsObject::GetRestitution)
    .set("SetRestitution", &PhysicsObject::SetRestitution)
    .set("GetMaxVelocity", &PhysicsObject::GetMaxVelocity)
    .set("SetMaxVelocity", &PhysicsObject::SetMaxVelocity)
    .set("IsStatic", &PhysicsObject::IsStatic)
    .set("SetStatic", &PhysicsObject::SetStatic)
    .set("IsPassable", &PhysicsObject::IsPassable)
    .set("SetPassable", &PhysicsObject::SetPassable)
    .set("IsActive", &PhysicsObject::IsActive)
    .set("SetActive", &PhysicsObject::SetActive)
    .set("AddForce", &PhysicsObject::AddForce);
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
 * @brief Add joint to list
 * @param aJoint Joint to add
 */
void PhysicsObject::AddJoint(Joint* aJoint)
{
  mJoints.push_back(aJoint);
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
void PhysicsObject::AddIgnore(HashString const &aObjectName)
{
  mIgnoreList[aObjectName.ToHash()] = aObjectName;
}

/**
 * @brief Remove object to ignore list
 * @param aObjectName Name of object to unignore
 */
void PhysicsObject::RemoveIgnore(HashString const &aObjectName)
{
  mIgnoreList.erase(aObjectName.ToHash());
}

/**
 * @brief Check if we should ignore the object in question
 * @param aObject The object to maybe ignore
 * @return Whether or not to ignore collision with this object
 */
bool PhysicsObject::IgnoreObject(GameObject const *aObject) const
{
  if(mIgnoreList.find(aObject->GetName().ToHash()) != mIgnoreList.end())
    return true;
    
  if(mIgnoreList.find(aObject->GetFileName().ToHash()) != mIgnoreList.end())
    return true;
    
  PhysicsObject::ConstIgnoreIT end = mIgnoreList.end();
  for(PhysicsObject::ConstIgnoreIT it = mIgnoreList.begin(); it != end; ++it)
  {
    if(aObject->HasTag(it->second))
    {
      return true;
    }
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
 * @brief Set velocity for this object
 * @param aVel Velocity to set to
 */
void PhysicsObject::SetVelocity(Vector3 const &aVel)
{
  mVelocity = aVel;
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
 * @brief Get inverse mass of this object
 * @return Mass
 */
float PhysicsObject::GetInverseMass() const
{
  return mInverseMass;
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
void PhysicsObject::SetRestitution(float const aRestitution)
{
  mRestitution = aRestitution;
}

/**
 * @brief Get max velocity for this object
 * @return Max velocity
 */
float PhysicsObject::GetMaxVelocity() const
{
  return mMaximumVelocity;
}

/**
 * @brief Set max velocity for this object
 * @param aMaximumVelocity Velocity to set to
 */
void PhysicsObject::SetMaxVelocity(float const aMaximumVelocity)
{
  mMaximumVelocity = aMaximumVelocity;
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
 * @brief Get if object is active (able to register collision)
 * @return Whether or not the object is active.
 */
bool PhysicsObject::IsActive() const
{
  return mActive;
}

/**
 * @brief Set if object is active (able to register collision)
 * @param aActive Whether or not the object is active.
 */
void PhysicsObject::SetActive(bool const aActive)
{
  mActive = aActive;
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
 * @brief Get list of ignorable objects
 * @return List of ignorable objects
 */
PhysicsObject::IgnoreContainer PhysicsObject::GetIgnoreList() const
{
  return mIgnoreList;
}

/**
 * @brief Set list of objects to ignore
 * @param aIgnoreList List of ignorable objects
 */
void PhysicsObject::SetIgnoreList(PhysicsObject::IgnoreContainer const &aIgnoreList)
{
  mIgnoreList = aIgnoreList;
}

/**
 * @brief Get all shapes
 * @return Shapes belonging to this object.
 */
PhysicsObject::ShapeContainer& PhysicsObject::GetShapes()
{
  return mShapes;
}

/**
 * @brief Get all joints
 * @return Joints belonging to this object.
 */
PhysicsObject::JointContainer& PhysicsObject::GetJoints()
{
  return mJoints;
}
