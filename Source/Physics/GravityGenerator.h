/*
 * GravityGenerator.h
 *
 *  Created on: Oct 13, 2012
 *      Author: jimmy
 */

#ifndef __JFramework_GravityGenerator_h_
#define __JFramework_GravityGenerator_h_

#include "ForceGenerator.h"

class GravityGenerator : public ForceGenerator
{
private:
public:
	GravityGenerator(Vector3 const &aForce);

	void Update(PhysicsObject *aObject, float aDuration);
};


#endif /* GRAVITYGENERATOR_H_ */
