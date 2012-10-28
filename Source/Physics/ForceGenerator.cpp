/*
 * ForceGenerator.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: jimmy
 */

#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(Vector3 const &aForce) : mForce(aForce), mActive(true)
{
}

Vector3 ForceGenerator::GetForce() const
{
	return mForce;
}

void ForceGenerator::SetActive(bool aActive)
{
	mActive = aActive;
}
