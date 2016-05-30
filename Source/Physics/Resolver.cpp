#include "Resolver.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "CollisionMessage.h"
#include "CollisionChecker.h"

Resolver::Resolver()
{
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
  std::unordered_map<size_t, PotentialPair>::iterator potentialEnd = mPotentialPairs.end();
  for(std::unordered_map<size_t, PotentialPair>::iterator it = mPotentialPairs.begin(); it != potentialEnd; ++it)
  {
    std::vector<CollisionPair> pairs = CollisionChecker::CheckShapeCollision(it->second);
    std::vector<CollisionPair>::iterator pairsEnd = pairs.end();
    for(std::vector<CollisionPair>::iterator it2 = pairs.begin(); it2 != pairsEnd; ++it2)
    {
      AddCollidedPair(*it2);
      SendCollisionMessages(*it2);
    }
  }
  
  std::list<CollisionPair>::iterator collidedEnd = mCollidedPairs.end();
  for(std::list<CollisionPair>::iterator it = mCollidedPairs.begin(); it != collidedEnd; ++it)
  {
    Resolve(*it, aDuration);
  }
  mCollidedPairs.clear();
  mPotentialPairs.clear();
}

/**
 * @brief Add a pair to be checked in pre check.
 * @param aPair Pair to add
 */
void Resolver::AddPrelimPair(PotentialPair const &aPair)
{
  mPotentialPairs[CalculateHash(aPair.mBodies[0], aPair.mBodies[1])] = aPair;
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
  CollisionMessage message("", aPair.mBodies[0]->GetOwner(), aPair.mBodies[1]->GetOwner(),
                           aPair.mShapes[0], aPair.mShapes[1]);
  aPair.mBodies[0]->GetOwner()->ReceiveMessage(message);
  aPair.mBodies[1]->GetOwner()->ReceiveMessage(message);
}

/**
 * @brief Resolve pairs
 * @param aPair Pair to resolve
 * @param aDuration Time between frames
 */
void Resolver::Resolve(CollisionPair &aPair, float aDuration)
{
  // Skip resolving if either object is passable.
  if(aPair.mBodies[0]->IsPassable() || aPair.mBodies[1]->IsPassable() ||
     aPair.mShapes[0]->passable || aPair.mShapes[1]->passable)
    return;
    
  switch(aPair.mShapes[0]->shape)
  {
  case Shape::SPHERE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateSphereToSphere(aPair);
      break;
    case Shape::CUBE:
      CalculateSphereToCube(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateTriangleToSphere(aPair);
      break;
    case Shape::LINE:
      CalculateLineToSphere(aPair);
      break;
    default:
      break;
    }
    break;
  case Shape::CUBE:
    switch(aPair.mShapes[1]->shape)
    {
    case Shape::SPHERE:
      CalculateSphereToCube(aPair);
      break;
    case Shape::CUBE:
      CalculateCubeToCube(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateTriangleToCube(aPair);
      break;
    case Shape::LINE:
      CalculateLineToCube(aPair);
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
    case Shape::CUBE:
      CalculateTriangleToCube(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateTriangleToTriangle(aPair);
      break;
    case Shape::LINE:
      CalculateLineToTriangle(aPair);
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
    case Shape::CUBE:
      CalculateLineToCube(aPair);
      break;
    case Shape::TRIANGLE:
      CalculateLineToTriangle(aPair);
      break;
    case Shape::LINE:
      CalculateLineToLine(aPair);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
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
  Vector3 b1Pos = b1Transform->GetPosition() + aPair.mShapes[0]->position.Multiply(b1Transform->GetScale());
  Vector3 b2Pos = b2Transform->GetPosition() + aPair.mShapes[1]->position.Multiply(b1Transform->GetScale());

  aPair.mPenetration = fabs((b1Pos - b2Pos).length() - 
    (aPair.mShapes[0]->GetSize(0) * b1Transform->GetScale().x + aPair.mShapes[1]->GetSize(0) * b2Transform->GetScale().x));
  aPair.mNormal = (b1Pos - b2Pos).normalize();
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Resolve sphere and cube
 * @param aPair Sphere and cube
 */
void Resolver::CalculateSphereToCube(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition() + aPair.mShapes[0]->position.Multiply(b1Transform->GetScale());
  Vector3 b2Pos = b2Transform->GetPosition() + aPair.mShapes[1]->position.Multiply(b2Transform->GetScale());
 
  int axis = 0;
  float shortestDistance = 0xffffff;
  
  for(int i = 0; i < 3; ++i)
  {
    float distance = fabs(fabs(b2Pos[i] - b1Pos[i]) - 
                      (aPair.mShapes[0]->GetSize(i) * b1Transform->GetScale().GetValue(i) + 
                      aPair.mShapes[1]->GetSize(i) * b2Transform->GetScale().GetValue(i)));
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

  aPair.mPenetration = fabs(fabs(b1Pos[axis] - b2Pos[axis]) - 
                        (aPair.mShapes[0]->GetSize(axis) * b1Transform->GetScale().GetValue(axis) + 
                        aPair.mShapes[1]->GetSize(axis) * b2Transform->GetScale().GetValue(axis)));
  aPair.mNormal = normal;
  aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
}

/**
 * @brief Resolve cubes
 * @param aPair Cubes
 */
void Resolver::CalculateCubeToCube(CollisionPair &aPair)
{
  Transform *b1Transform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *b2Transform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 b1Pos = b1Transform->GetPosition() + aPair.mShapes[0]->position.Multiply(b1Transform->GetScale());
  Vector3 b2Pos = b2Transform->GetPosition() + aPair.mShapes[1]->position.Multiply(b2Transform->GetScale());
  
  int axis = 0;
  float shortestDistance = 0xffffff;
  
  for(int i = 0; i < 3; ++i)
  {
    float distance = fabs(fabs(b2Pos[i] - b1Pos[i]) - 
                      (aPair.mShapes[0]->GetSize(i) * b1Transform->GetScale().GetValue(i) + 
                      aPair.mShapes[1]->GetSize(i) * b2Transform->GetScale().GetValue(i)));
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
  
  aPair.mPenetration = fabs(fabs(b1Pos[axis] - b2Pos[axis]) - 
                        (aPair.mShapes[0]->GetSize(axis) * b1Transform->GetScale().GetValue(axis) + 
                        aPair.mShapes[1]->GetSize(axis) * b2Transform->GetScale().GetValue(axis)));
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
  {
    aPair.Switch();
  }
  
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  Sphere* sphere = (Sphere*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 spherePos = sphere->position.Multiply(sphereTransform->GetScale()) + sphereTransform->GetPosition();
  Vector3 closestPoint = triangle->GetClosestPointToTriangle(triTransform->GetPosition(), spherePos);
  
  Vector3 dist = closestPoint - spherePos;
  float size = sphere->GetSize(0) * sphereTransform->GetScale().x;
  if(dist.length() < size)
  {
    aPair.mPenetration = size - dist.length();
    aPair.mNormal = dist.normalize();
    aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
  }
}

/**
 * @brief Resolve Triangle and cube. Work in progress.
 * @param aPair Triangle and cube.
 */
void Resolver::CalculateTriangleToCube(CollisionPair &aPair)
{
  // Ordering is important
  if(aPair.mShapes[0]->shape != Shape::TRIANGLE)
  {
    aPair.Switch();
  }
  
  // TODO doesn't work check orange collision book
  Triangle* triangle = (Triangle*)aPair.mShapes[0];
  Cube* cube = (Cube*)aPair.mShapes[1];
  Transform* triTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform* cubeTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  Vector3 cubePos = cube->position.Multiply(cubeTransform->GetScale()) + cubeTransform->GetPosition();
  Vector3 closestPoint = triangle->GetClosestPointToTriangle(triTransform->GetPosition(), cubePos);
  Vector3 dist = closestPoint - cubePos;
  int axis = 0;
  float minDistance = 0xffffff;
  
  for(int i = 0; i < 3; ++i)
  {
    if(fabs(dist[i]) - cube->GetSize(i) < minDistance)
    {
      axis = i;
      minDistance = fabs(dist[i]) - cube->GetSize(i);
    }
  }
  
  Vector3 normal;
  // Figure out the normal
  switch(axis)
  {
    case 0:
      normal = Vector3(minDistance,0,0).normalize();
      break;
    case 1:
      normal = Vector3(0,minDistance,0).normalize();
      break;
    case 2:
      normal = Vector3(0,0,minDistance).normalize();
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
  assert(!"TODO");
}

/**
 * @brief Calculate Line to Sphere resolution
 * @param aPair Line and Sphere
 */
void Resolver::CalculateLineToSphere(CollisionPair &aPair)
{
  if(aPair.mShapes[0]->shape == Shape::SPHERE)
  {
    aPair.Switch();
  }
  
  // Basic set up
  Line *line = (Line*)aPair.mShapes[0];
  Sphere *sphere = (Sphere*)aPair.mShapes[1];
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *sphereTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  // This is a bit slow since it makes a new line every time.
  // Move line into world space.
  Line tempLine = (*line);
  tempLine.direction = tempLine.direction.Multiply(lineTransform->GetScale());
  tempLine.position = tempLine.position.Multiply(lineTransform->GetScale());
  tempLine.position += lineTransform->GetPosition();
  
  // Find closest point on line to sphere pos and calculate penetration
  // based on radius.
  Vector3 spherePos = sphereTransform->GetPosition() + sphere->position.Multiply(sphereTransform->GetScale());
  Vector3 closestPoint = tempLine.ClosestPointToPoint(spherePos);
  Vector3 dist = closestPoint - spherePos;
  float radius = sphere->GetSize(0) * sphereTransform->GetScale().x;
  if(dist.length() < radius)
  {
    aPair.mPenetration = radius - dist.length();
    aPair.mNormal = dist.normalize();
    aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
  }
}

/**
 * @brief Calculate Line to Cube resolution
 * @param aPair Line and Cube
 */
void Resolver::CalculateLineToCube(CollisionPair &aPair)
{
  // TODO this doesn't work, but I'll leave it in so at least something happens.
  if(aPair.mShapes[0]->shape == Shape::CUBE)
  {
    aPair.Switch();
  }
  
  // Basic set up
  Line *line = (Line*)aPair.mShapes[0];
  Cube *cube = (Cube*)aPair.mShapes[1];
  Transform *lineTransform = aPair.mBodies[0]->GetOwner()->GET<Transform>();
  Transform *cubeTransform = aPair.mBodies[1]->GetOwner()->GET<Transform>();
  
  // This is a bit slow since it makes a new line every time.
  // Move line into world space.
  Line tempLine = (*line);
  tempLine.direction = tempLine.direction.Multiply(lineTransform->GetScale());
  tempLine.position = tempLine.position.Multiply(lineTransform->GetScale());
  tempLine.position += lineTransform->GetPosition();
  
  // Find closest point on line to sphere pos and calculate penetration
  // based on radius.
  Vector3 cubePos = cubeTransform->GetPosition() + cube->position.Multiply(cubeTransform->GetScale());
  Vector3 closestPoint = tempLine.ClosestPointToPoint(cubePos);
  Vector3 dist = closestPoint - cubePos;
  
  // Push out on smallest axis
  aPair.mPenetration = FLT_MAX;
  for(int i = 0; i < 3; ++i)
  {
    float size = cube->GetSize(i) * cubeTransform->GetScale().GetValue(i);
    float diff = size - dist.length();
    if(aPair.mPenetration > fabs(diff))
    {
      aPair.mPenetration = diff;
      aPair.mRelativeVelocity = aPair.mBodies[0]->GetVelocity() - aPair.mBodies[1]->GetVelocity();
      
      // TODO get the correct normal
      switch(i)
      {
      case 0:
        aPair.mNormal = Vector3(1,0,0);
        break;
      case 1:
        aPair.mNormal = Vector3(0,1,0);
        break;
      case 2:
        aPair.mNormal = Vector3(0,0,1);
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
  // TODO
  //assert(!"TODO");
}

/**
 * @brief Calculate Line to Line resolution
 * @param aPair Both Lines
 */
void Resolver::CalculateLineToLine(CollisionPair &aPair)
{
  // TODO
  //assert(!"TODO");
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