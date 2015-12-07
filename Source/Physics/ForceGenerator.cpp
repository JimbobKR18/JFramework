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

ForceGenerator::~ForceGenerator()
{
}

/**
 * @brief Get force.
 */
Vector3 ForceGenerator::GetForce() const
{
	return mForce;
}

/**
 * @brief Set whether of not this generator is active.
 * @param aActive
 */
void ForceGenerator::SetActive(bool aActive)
{
	mActive = aActive;
}
