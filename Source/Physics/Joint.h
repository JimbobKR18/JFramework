#ifndef __JFramework_Joint_h_
#define __JFramework_Joint_h_

#include "Common.h"
#include "MathExt.h"

class PhysicsObject;
class Joint
{
private:
  int mId;
  Vector3 mPosition;
  PhysicsObject *mOwner;
  
public:
  Joint(int aId, Vector3 const &aPosition, PhysicsObject *aOwner);
  virtual ~Joint();
  
  // GETTERS
  int GetID() const;
  Vector3 const& GetPosition() const;
  PhysicsObject *GetOwner() const;
  
  // SETTERS
  void SetID(int const aId);
  void SetPosition(Vector3 const &aPosition);
};

#endif // __JFramework_Joint_h_
