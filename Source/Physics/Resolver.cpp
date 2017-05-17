#include "Resolver.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "CollisionMessage.h"
#include "CollisionChecker.h"
#include "ShapeMath.h"
#include "Constants.h"

Resolver::Resolver() : mResolveAxes(3), mCollidedPairs(), mPotentialPairs(), mSortedPairs()
{
  if(Constants::GetBoolean("2DCollisionOnly"))
    mResolveAxes = 2;
}

Resolver::~Resolver()
{
}

/**
 * @brief Update loop for resolving shapes.
 * @param aDuration Time elapsed between flames.
 */
void Resolver::Update(float aDuration)
{
  std::set<PotentialPair>::iterator potentialEnd = mSortedPairs.end();
  for(std::set<PotentialPair>::iterator it = mSortedPairs.begin(); it != potentialEnd; ++it)
  {
    std::vector<CollisionPair> pairs = CollisionChecker::CheckShapeCollision(*it);
    std::vector<CollisionPair>::iterator pairsEnd = pairs.end();
    for(std::vector<CollisionPair>::iterator it2 = pairs.begin(); it2 != pairsEnd; ++it2)
    {
      Resolve(*it2, aDuration);
      SendCollisionMessages(*it2);
    }
  }
  
  mCollidedPairs.clear();
  mPotentialPairs.clear();
  mSortedPairs.clear();
}

/**
 * @brief Add a pair to be checked in pre check.
 * @param aPair Pair to add
 */
void Resolver::AddPrelimPair(PotentialPair const &aPair)
{
  mPotentialPairs[CalculateHash(aPair.mBodies[0], aPair.mBodies[1])] = aPair;
  mSortedPairs.insert(aPair);
}

/**
 * @brief Add pair that collided.
 * @param aPair The pair to add.
 */
void Resolver::AddCollidedPair(CollisionPair const &aPair)
{
  mCollidedPairs.push_back(aPair);
}

/**
 * @brief Find a pair in our list of potential pairs.
 * @param aObject1 First object
 * @param aObject2 Second object
 * @return Whether or not that pair already exists
 */
bool Resolver::Find(PhysicsObject *aObject1, PhysicsObject *aObject2)
{
  return mPotentialPairs.find(CalculateHash(aObject1, aObject2)) != mPotentialPairs.end();
}

/**
 * @brief Calculate separating velocities
 * @param aPair Pair to calculate
 * @return The separating velocity along the normal.
 */
float Resolver::CalculateSeparatingVelocity(CollisionPair const &aPair)
{
  Vector3 separatingVelocity = aPair.mBodies[0]->GetVelocity();
  if(aPair.mBodies[1])
    separatingVelocity -= aPair.mBodies[1]->GetVelocity();
  return separatingVelocity * aPair.mNormal;
}

/**
 * @brief Resolve penetration of two objects
 * @param aPair Pair ro resolve
 */
void Resolver::ResolvePenetration(CollisionPair const &aPair)
{
  if(aPair.mPenetration <= 0)
    return;
    
  PhysicsObject *body1 = aPair.mBodies[0];
  PhysicsObject *body2 = aPair.mBodies[1];
  Transform *transform1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *transform2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();

  float totalInverseMass = body1->GetInverseMass() + body2->GetInverseMass();

  if(totalInverseMass <= 0)
    return;

  // Find the rate in which each object must move
  Vector3 movePerIMass = aPair.mNormal * (aPair.mPenetration / totalInverseMass);
  Vector3 b1Movement = movePerIMass * body1->GetInverseMass();
  Vector3 b2Movement = movePerIMass * body2->GetInverseMass();

  // Must check if objects can be moved
  if(!body1->IsStatic())
    transform1->SetPosition(transform1->GetPosition() + b1Movement);
  if(!body2->IsStatic())
    transform2->SetPosition(transform2->GetPosition() - b2Movement);
}

/**
 * @brief Resolve for velocities
 * @param aPair Pair to resolve
 * @param aDuration Length of frame
 */
void Resolver::ResolveVelocity(CollisionPair const &aPair, float aDuration)
{
  float separatingVelocity = CalculateSeparatingVelocity(aPair);
  
  if(separatingVelocity > 0)
    return;
    
  PhysicsObject *body1 = aPair.mBodies[0];
  PhysicsObject *body2 = aPair.mBodies[1];
  
  float restitution1 = body1->GetRestitution();
  float restitution2 = body2->GetRestitution();
  float newSeparatingVelocity1 = -separatingVelocity * restitution1;
  float newSeparatingVelocity2 = -separatingVelocity * restitution2;
  
  Vector3 accCausedVelocity = body1->GetAcceleration();
  if(body2)
    accCausedVelocity -= body2->GetAcceleration();
  
  float accCausedSepVelocity = accCausedVelocity * aPair.mNormal * aDuration;
  
  if(accCausedSepVelocity < 0)
  {
    newSeparatingVelocity1 += restitution1 * accCausedSepVelocity;
    newSeparatingVelocity2 += restitution2 * accCausedSepVelocity;
    if(newSeparatingVelocity1 < 0)
      newSeparatingVelocity1 = 0;
    if(newSeparatingVelocity2 < 0)
      newSeparatingVelocity2 = 0;
  }
  
  float deltaVelocity1 = newSeparatingVelocity1 - separatingVelocity;
  float deltaVelocity2 = newSeparatingVelocity2 - separatingVelocity;
  float totalInverseMass = 0.0f;
  if(body1)
    totalInverseMass += body1->GetInverseMass();
  if(aPair.mBodies[1])
    totalInverseMass += body2->GetInverseMass();
  
  if(totalInverseMass <= 0) return;
  
  float impulse1 = deltaVelocity1 / totalInverseMass;
  float impulse2 = deltaVelocity2 / totalInverseMass;
  Vector3 impulsePerIMass1 = aPair.mNormal * impulse1;
  Vector3 impulsePerIMass2 = aPair.mNormal * impulse2;
  
  Vector3 b1Movement = impulsePerIMass1 * body1->GetInverseMass();
  Vector3 b2Movement = impulsePerIMass2 * body2->GetInverseMass();
  
  if(body1 && !body1->IsStatic())
    body1->SetVelocity(body1->GetVelocity() + b1Movement);
  
  if(body2 && !body2->IsStatic())
    body2->SetVelocity(body2->GetVelocity() - b2Movement);
}

/**
 * @brief Notify object of collision
 * @param aPair Pair to message
 */
void Resolver::SendCollisionMessages(CollisionPair &aPair) const
{
  CollisionMessage message1("", aPair.mBodies[0]->GetOwner(), aPair.mBodies[1]->GetOwner(),
                           aPair.mShapes[0], aPair.mShapes[1], aPair.mNormal, aPair.mRelativeVelocity,
                           aPair.mPenetration);
  CollisionMessage message2("", aPair.mBodies[0]->GetOwner(), aPair.mBodies[1]->GetOwner(),
                           aPair.mShapes[0], aPair.mShapes[1], aPair.mNormal.Invert(), aPair.mRelativeVelocity.Invert(),
                           aPair.mPenetration);
  aPair.mBodies[0]->GetOwner()->ReceiveMessage(message1);
  aPair.mBodies[1]->GetOwner()->ReceiveMessage(message2);
}

/**
 * @brief Resolve pairs
 * @param aPair Pair to resolve
 * @param aDuration Time between frames
 */
void Resolver::Resolve(CollisionPair &aPair, float aDuration)
{
  // Need to calculate physics params before resolution.
  switch(aPair.mShapes[0]->shape)
  {
  case Shape::SPHERE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateSphereToSphere(aPair);
      break;
    case Shape::AABB:
      CalculateSphereToAABB(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateTriangleToSphere(aPair);
      break;
    case Shape::LINE:
      CalculateLineToSphere(aPair);
      break;
    case Shape::OBB:
      CalculateOBBToSphere(aPair);
      break;
    default:
      break;
    }
    break;
  case Shape::AABB:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateSphereToAABB(aPair);
      break;
    case Shape::AABB:
      CalculateAABBToAABB(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateTriangleToAABB(aPair);
      break;
    case Shape::LINE:
      CalculateLineToAABB(aPair);
      break;
    case Shape::OBB:
      CalculateOBBToAABB(aPair);
      break;
    default:
      break;
    }
    break;
  case Shape::TRIANGLE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateTriangleToSphere(aPair);
      break;
    case Shape::AABB:
      CalculateTriangleToAABB(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateTriangleToTriangle(aPair);
      break;
    case Shape::LINE:
      CalculateLineToTriangle(aPair);
      break;
    case Shape::OBB:
      CalculateOBBToTriangle(aPair);
      break;
    default:
      break;
    }
    break;
  case Shape::LINE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateLineToSphere(aPair);
      break;
    case Shape::AABB:
      CalculateLineToAABB(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateLineToTriangle(aPair);
      break;
    case Shape::LINE:
      CalculateLineToLine(aPair);
      break;
    case Shape::OBB:
      CalculateOBBToLine(aPair);
      break;
    default:
      break;
    }
    break;
  case Shape::OBB:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateOBBToSphere(aPair);
      break;
    case Shape::AABB:
      CalculateOBBToAABB(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateOBBToTriangle(aPair);
      break;
    case Shape::LINE:
      CalculateOBBToLine(aPair);
      break;
    case Shape::OBB:
      CalculateOBBToOBB(aPair);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  
  // Skip resolving if either object is passable.
  if(aPair.mBodies[0]->IsPassable() || aPair.mBodies[1]->IsPassable() ||
     aPair.mShapes[0]->passable || aPair.mShapes[1]->passable)
    return;
  
  ResolveVelocity(aPair, aDuration);
  ResolvePenetration(aPair);
}

/**
 * @brief Resolve sphere.
 * @param aPair Pair of spheres
 */
void Resolver::CalculateSphereToSphere(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = ShapeMath::GetLocalCoordinates(b1Transform, aPair.mShapes[0]->position);
  Vector3 b2Pos = ShapeMath::GetLocalCoordinates(b2Transform, aPair.mShapes[1]->position);

  aPair.mPenetration = fabs((b1Pos - b2Pos).length() - 
    (aPair.mShapes[0]->GetSize(0) * b1Transform->GetHierarchicalScale().x + aPair.mShapes[1]->GetSize(0) * b2Transform->GetHierarchicalScale().x));
  aPair.mNormal = (b1Pos - b2Pos).normalize();
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Resolve sphere and aabb
 * @param aPair Sphere and aabb
 */
void Resolver::CalculateSphereToAABB(CollisionPair &aPair)
{
  // Ordering is important
  if(aPair.mShapes[0]->shape != Shape::SPHERE)
    aPair.Switch();
  
  Transform *sphere = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *aabb = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphere, aPair.mShapes[0]->position);
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabb, aPair.mShapes[1]->position);
  Vector3 aabbSize = dynamic_cast<AxisAlignedBoundingBox*>(aPair.mShapes[1])->size.Multiply(aabb->GetHierarchicalScale());
  Vector3 closestPoint = ShapeMath::ClosestPointPointAABB(spherePos, aabbPos - aabbSize, aabbPos + aabbSize);
  Vector3 dist = closestPoint - spherePos;
  float size = aPair.mShapes[0]->GetSize(0) * sphere->GetHierarchicalScale().x;
  
  int axis = 0;
  float shortestDistance = 0xffffff;
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float distance = fabs(size - fabs(dist[i]));
    if(distance < shortestDistance)
    {
      axis = i;
      shortestDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(spherePos[axis] - aabbPos[axis],0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,spherePos[axis] - aabbPos[axis],0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,spherePos[axis] - aabbPos[axis]).normalize();
      break;
  }

  aPair.mPenetration = shortestDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Resolve aabbs
 * @param aPair AABBs
 */
void Resolver::CalculateAABBToAABB(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = ShapeMath::GetLocalCoordinates(b1Transform, aPair.mShapes[0]->position);
  Vector3 b2Pos = ShapeMath::GetLocalCoordinates(b2Transform, aPair.mShapes[1]->position);
  
  int axis = 0;
  float shortestDistance = 0xffffff;
  
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float distance = fabs(fabs(b2Pos[i] - b1Pos[i]) - 
                      (aPair.mShapes[0]->GetSize(i) * b1Transform->GetHierarchicalScale().GetValue(i) + 
                      aPair.mShapes[1]->GetSize(i) * b2Transform->GetHierarchicalScale().GetValue(i)));
    if(distance < shortestDistance)
    {
      axis = i;
      shortestDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(b1Pos[axis] - b2Pos[axis],0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,b1Pos[axis] - b2Pos[axis],0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,b1Pos[axis] - b2Pos[axis]).normalize();
      break;
  }
  
  aPair.mPenetration = shortestDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Resolve triangle and sphere
 * @param aPair Triangle and sphere
 */
void Resolver::CalculateTriangleToSphere(CollisionPair &aPair)
{
  // Ordering is important
  if(aPair.mShapes[0]->shape != Shape::TRIANGLE)
    aPair.Switch();
  
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  Sphere* sphere = (Sphere*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphereTransform, sphere->position);
  Vector3 a = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(0));
  Vector3 b = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(1));
  Vector3 c = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(2));
  Vector3 closestPoint = ShapeMath::ClosestPointPointTriangle(spherePos, a, b, c);
  
  Vector3 dist = closestPoint - spherePos;
  float size = sphere->GetSize(0) * sphereTransform->GetHierarchicalScale().x;
  if(dist.length() < size)
  {
    aPair.mPenetration = size - dist.length();
    aPair.mNormal = dist.normalize();
    aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
  }
}

/**
 * @brief Resolve Triangle and aabb. Work in progress.
 * @param aPair Triangle and aabb.
 */
void Resolver::CalculateTriangleToAABB(CollisionPair &aPair)
{
  // Ordering is important
  if(aPair.mShapes[0]->shape != Shape::TRIANGLE)
    aPair.Switch();
  
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  AxisAlignedBoundingBox* aabb = (AxisAlignedBoundingBox*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* aabbTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabbTransform, aabb->position);
  Vector3 a = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(0));
  Vector3 b = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(1));
  Vector3 c = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(2));
  Vector3 closestPoint = ShapeMath::ClosestPointPointTriangle(aabbPos, a, b, c);
  Vector3 dist = closestPoint - aabbPos;
  int axis = 0;
  float minDistance = 0xffffff;
  
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float size = aabb->GetSize(i) * aabbTransform->GetHierarchicalScale().GetValue(i);
    float distance = fabs(fabs(dist[i]) - size);
    if(distance < minDistance)
    {
      axis = i;
      minDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(dist.x,0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,dist.y,0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,dist.z).normalize();
      break;
  }
  
  aPair.mPenetration = minDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Calculate triangles. Work in progress.
 * @param aPair Triangles.
 */
void Resolver::CalculateTriangleToTriangle(CollisionPair &aPair)
{
  DebugLogPrint("TODO: CalculateTriangleToTriangle\n");
}

/**
 * @brief Calculate Line to Sphere resolution
 * @param aPair Line and Sphere
 */
void Resolver::CalculateLineToSphere(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::SPHERE)
    aPair.Switch();
  
  // Basic set up
  Line *line = (Line*)aPair.mShapes[0];
  Sphere *sphere = (Sphere*)aPair.mShapes[1];
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  // Find closest point on line to sphere pos and calculate penetration
  // based on radius.
  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphereTransform, sphere->position);
  Vector3 linePos = ShapeMath::GetLocalCoordinates(lineTransform, line->position);
  Vector3 lineEnd = linePos + ((line->direction.Multiply(lineTransform->GetHierarchicalRotation() * lineTransform->GetHierarchicalScale()) * line->length));
  Vector3 closestPoint = ShapeMath::ClosestPointPointSegment(spherePos, linePos, lineEnd);
  Vector3 dist = closestPoint - spherePos;
  float radius = sphere->GetSize(0) * sphereTransform->GetHierarchicalScale().x;
  if(dist.length() < radius)
  {
    aPair.mPenetration = radius - dist.length();
    aPair.mNormal = dist.normalize();
    aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
  }
}

/**
 * @brief Calculate Line to AABB resolution
 * @param aPair Line and AABB
 */
void Resolver::CalculateLineToAABB(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::AABB)
    aPair.Switch();
  
  // Basic set up
  Line *line = (Line*)aPair.mShapes[0];
  AxisAlignedBoundingBox *aabb = (AxisAlignedBoundingBox*)aPair.mShapes[1];
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *aabbTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  // Find closest point on line to sphere pos and calculate penetration
  // based on radius.
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabbTransform, aabb->position);
  Vector3 linePos = ShapeMath::GetLocalCoordinates(lineTransform, line->position);
  Vector3 lineEnd = linePos + ((line->direction.Multiply(lineTransform->GetHierarchicalRotation() * lineTransform->GetHierarchicalScale()) * line->length));
  Vector3 closestPoint = ShapeMath::ClosestPointPointSegment(aabbPos, linePos, lineEnd);
  Vector3 dist = closestPoint - aabbPos;
  
  // Push out on smallest axis
  aPair.mPenetration = FLT_MAX;
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float size = aabb->GetSize(i) * aabbTransform->GetHierarchicalScale().GetValue(i);
    float diff = size - dist.length();
    if(aPair.mPenetration > fabs(diff))
    {
      aPair.mPenetration = diff;
      aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
      
      switch(i)
      {
      case 0:
        aPair.mNormal = Vector3(dist.x,0,0).normalize();
        break;
      case 1:
        aPair.mNormal = Vector3(0,dist.y,0).normalize();
        break;
      case 2:
        aPair.mNormal = Vector3(0,0,dist.z).normalize();
        break;
      default:
        assert(!"How did you even get here?");
        break;
      }
    }
  }
}

/**
 * @brief Calculate Line to Triangle resolution
 * @param aPair Line and Triangle
 */
void Resolver::CalculateLineToTriangle(CollisionPair &aPair)
{
  DebugLogPrint("TODO: CalculateLineToTriangle\n");
}

/**
 * @brief Calculate Line to Line resolution
 * @param aPair Both Lines
 */
void Resolver::CalculateLineToLine(CollisionPair &aPair)
{
  Transform *t1 = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *t2 = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Line *l1 = dynamic_cast<Line*>(aPair.mShapes[0]);
  Line *l2 = dynamic_cast<Line*>(aPair.mShapes[1]);
  
  Vector3 a = ShapeMath::GetLocalCoordinates(t1, l1->position);
  Vector3 b = a + (l1->direction.Multiply(t1->GetHierarchicalRotation() * t1->GetHierarchicalScale()) * l1->length);
  Vector3 c = ShapeMath::GetLocalCoordinates(t2, l2->position);
  Vector3 d = c + (l2->direction.Multiply(t2->GetHierarchicalRotation() * t2->GetHierarchicalScale()) * l2->length);
  
  float a1 = ShapeMath::Signed2DTriArea(a, b, d);
  float a2 = ShapeMath::Signed2DTriArea(a, b, c);
  float a3 = ShapeMath::Signed2DTriArea(c, d, a);
  float a4 = a3 + a2 - a1;
    
  float t = a3 / (a3 - a4);
  Vector3 p = a + ((b - a) * t);
  
  aPair.mPenetration = p.length();
  aPair.mNormal = (p - a).normalize();
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Calculate OBB to Sphere resolution.
 * @param aPair OBB and Sphere.
 */
void Resolver::CalculateOBBToSphere(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::SPHERE)
    aPair.Switch();
    
  OrientedBoundingBox *obb = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  Sphere *sphere = dynamic_cast<Sphere*>(aPair.mShapes[1]);
  Transform* obbTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 orientation[3];
  orientation[0] = obbTransform->GetHierarchicalRotation() * obb->right;
  orientation[1] = obbTransform->GetHierarchicalRotation() * obb->up;
  orientation[2] = obbTransform->GetHierarchicalRotation() * obb->forward;
  Vector3 spherePos = ShapeMath::GetLocalCoordinates(sphereTransform, sphere->position);
  Vector3 obbPos = ShapeMath::GetLocalCoordinates(obbTransform, obb->position);
  Vector3 diff = obbPos - spherePos;
  float shortestDistance = FLT_MAX;
  float shortestAxis = 0;
  
  for(int i = 0; i < mResolveAxes; ++i)
  {
    Vector3 axis = obbTransform->GetHierarchicalRotation() * obb->GetAxis(i);
    Vector3 projection = axis * (diff.Dot(axis) / axis.Dot(axis));
    float size = obb->GetSize(i) * obbTransform->GetHierarchicalScale().GetValue(i);
    float distance = fabs(projection.length() - size);
    if(distance < shortestDistance)
    {
      shortestAxis = i;
      shortestDistance = distance;
    }
  }
  
  Vector3 axis = obbTransform->GetHierarchicalRotation() * obb->GetAxis(shortestAxis);
  Vector3 normal = (axis * diff.Dot(axis)).normalize();
  aPair.mPenetration = shortestDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Calculate OBB to AABB resolution.
 * @param aPair OBB and AABB.
 */
void Resolver::CalculateOBBToAABB(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::AABB)
    aPair.Switch();
    
  OrientedBoundingBox *obb = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  AxisAlignedBoundingBox *aabb = dynamic_cast<AxisAlignedBoundingBox*>(aPair.mShapes[1]);
  Transform* obbTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* aabbTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 orientation[3];
  orientation[0] = obbTransform->GetHierarchicalRotation() * obb->right;
  orientation[1] = obbTransform->GetHierarchicalRotation() * obb->up;
  orientation[2] = obbTransform->GetHierarchicalRotation() * obb->forward;
  Vector3 aabbPos = ShapeMath::GetLocalCoordinates(aabbTransform, aabb->position);
  Vector3 obbPos = ShapeMath::GetLocalCoordinates(obbTransform, obb->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(aabbPos, obbPos, orientation, obbTransform->GetHierarchicalScale().Multiply(obb->extents));
  Vector3 diff = closestPoint - aabbPos;
  
  int axis = 0;
  float minDistance = 0xffffff;
  
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float size = aabb->GetSize(i) * aabbTransform->GetHierarchicalScale().GetValue(i);
    float distance = fabs(fabs(diff[i]) - size);
    if(distance < minDistance)
    {
      axis = i;
      minDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(diff.x,0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,diff.y,0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,diff.z).normalize();
      break;
  }
  
  aPair.mPenetration = minDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Calculate OBB to OBB resolution.
 * @param aPair Both OBBs.
 */
void Resolver::CalculateOBBToOBB(CollisionPair &aPair)
{
  OrientedBoundingBox *obb1 = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  OrientedBoundingBox *obb2 = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[1]);
  Transform* obb1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* obb2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 orientation[3];
  orientation[0] = obb2Transform->GetHierarchicalRotation() * obb2->right;
  orientation[1] = obb2Transform->GetHierarchicalRotation() * obb2->up;
  orientation[2] = obb2Transform->GetHierarchicalRotation() * obb2->forward;
  Vector3 obb1Pos = ShapeMath::GetLocalCoordinates(obb1Transform, obb1->position);
  Vector3 obb2Pos = ShapeMath::GetLocalCoordinates(obb2Transform, obb2->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(obb1Pos, obb2Pos, orientation, obb2Transform->GetHierarchicalScale().Multiply(obb2->extents));
  Vector3 diff = closestPoint - obb1Pos;
  
  int axis = 0;
  float minDistance = 0xffffff;
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float size = obb1->extents[i] * obb1Transform->GetHierarchicalScale().GetValue(i);
    float d = diff.Dot(obb1Transform->GetHierarchicalRotation() * obb1->GetAxis(i));
    float distance = fabs(fabs(d) - size);
    if(distance < minDistance)
    {
      axis = i;
      minDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = obb1Transform->GetHierarchicalRotation() * obb1->right;
      break;
    case 1:
      normal = obb1Transform->GetHierarchicalRotation() * obb1->up;
      break;
    case 2:
      normal = obb1Transform->GetHierarchicalRotation() * obb1->forward;
      break;
  }
  normal = (normal * -diff.Dot(normal)).normalize();
  
  aPair.mPenetration = minDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Calculate OBB to Triangle resolution.
 * @param aPair OBB and Triangle.
 */
void Resolver::CalculateOBBToTriangle(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::TRIANGLE)
    aPair.Switch();
    
  OrientedBoundingBox *obb = dynamic_cast<OrientedBoundingBox*>(aPair.mShapes[0]);
  Triangle *triangle = dynamic_cast<Triangle*>(aPair.mShapes[1]);
  Transform* obbTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* triTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  Vector3 a = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(0));
  Vector3 b = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(1));
  Vector3 c = ShapeMath::GetLocalCoordinates(triTransform, triangle->GetPoint(2));
  Vector3 obbPos = ShapeMath::GetLocalCoordinates(obbTransform, obb->position);
  Vector3 closestPoint = ShapeMath::ClosestPointPointTriangle(obbPos, a, b, c);
  Vector3 diff = closestPoint - obbPos;
  
  int axis = 0;
  float minDistance = 0xffffff;
  for(int i = 0; i < mResolveAxes; ++i)
  {
    float size = obb->extents[i] * obbTransform->GetHierarchicalScale().GetValue(i);
    float d = diff.Dot(obbTransform->GetHierarchicalRotation() * obb->GetAxis(i));
    float distance = fabs(fabs(d) - size);
    if(distance < minDistance)
    {
      axis = i;
      minDistance = distance;
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = obbTransform->GetHierarchicalRotation() * obb->right;
      break;
    case 1:
      normal = obbTransform->GetHierarchicalRotation() * obb->up;
      break;
    case 2:
      normal = obbTransform->GetHierarchicalRotation() * obb->forward;
      break;
  }
  normal = (normal * -diff.Dot(normal)).normalize();
  
  aPair.mPenetration = minDistance;
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Calculate OBB to Line resolution.
 * @param aPair OBB and Line.
 */
void Resolver::CalculateOBBToLine(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::LINE)
    aPair.Switch();
  // TODO
}

/**
 * @brief Calculate hash between two bodies.
 * @param aObject1 Body 1
 * @param aObject2 Body 2
 * @return Hash value.
 */
size_t Resolver::CalculateHash(PhysicsObject *aObject1, PhysicsObject *aObject2)
{
  return (size_t)aObject1 + (size_t)aObject2 + 
    aObject1->GetOwner()->GetName().ToHash() + aObject2->GetOwner()->GetName().ToHash();
}