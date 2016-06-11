/*
 * ForceRegistry.h
 *
 *  Created on: Oct 13, 2012
 *      Author: jimmy
 */

#ifndef __JFramework_ForceRegistry_h_
#define __JFramework_ForceRegistry_h_

#include "ForceGenerator.h"
#include <vector>

struct ForceRegistration
{
  PhysicsObject*  mObject;
  ForceGenerator* mGenerator;

  ForceRegistration(PhysicsObject *aObject, ForceGenerator *aGenerator) : mObject(aObject), mGenerator(aGenerator) {}
};

class ForceRegistry
{
private:
  std::vector<ForceRegistration> mRegistry;
public:
  ForceRegistry();
  ~ForceRegistry();

  void Update();

  void Add(PhysicsObject *aObject, ForceGenerator *aGenerator);
  void Remove(PhysicsObject *aObject, ForceGenerator *aGenerator);
  void RemoveForcesForObject(PhysicsObject *aObject);
  void Clear();
};

#endif /* FORCEREGISTRY_H_ */
