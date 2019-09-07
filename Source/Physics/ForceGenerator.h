/*
 * ForceGenerator.h
 *
 *  Created on: Oct 13, 2012
 *      Author: jimmy
 */

#ifndef __JFramework_ForceGenerator_h_
#define __JFramework_ForceGenerator_h_

#include "MathExt.h"

class PhysicsObject;

class ForceGenerator
{
private:
	Vector3 mForce;
	bool    mActive;
public:
	ForceGenerator(Vector3 const &aForce);
    virtual ~ForceGenerator();

	virtual void   Update(PhysicsObject *aObject, float aDuration) = 0;
	Vector3 const& GetForce() const;
	void           SetActive(bool aActive);
};


#endif /* FORCEGENERATOR_H_ */
