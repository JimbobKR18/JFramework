#ifndef __JFramework_Constraint_h_
#define __JFramework_Constraint_h_

#include "Common.h"
#include "MathExt.h"
#include "Component.h"
#include "Joint.h"

class Constraint : public Component
{
private:
  Joint* mJoints[2];
  static int const sUID;

protected:
  Constraint(int const &aUID);

public:
  Constraint();
  virtual ~Constraint();
  
  Joint*              GetJoint(int aId);
  
  virtual void        Update() = 0;
  virtual void        SendMessage(Message const &aMessage) = 0;
  virtual void        ReceiveMessage(Message const &aMessage) = 0;
  virtual void        Serialize(Parser &aParser) = 0;
  virtual void        Deserialize(ParserNode *aNode) = 0;
  static int          GetUID() {return sUID;}
};

#endif // __JFramework_Constraint_h_