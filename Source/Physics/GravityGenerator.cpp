/*
 * GravityGenerator.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: jimmy
 */

#include "GravityGenerator.h"
#include "PhysicsObject.h"

GravityGenerator::GravityGenerator(Vector3 const &aForce) : ForceGenerator(aForce)
{
}

GravityGenerator::~GravityGenerator()
{
}

void GravityGenerator::Update(PhysicsObject *aObject, float aDuration)
{
	aObject->AddForce(GetForce() * aObject->GetMass());
}


