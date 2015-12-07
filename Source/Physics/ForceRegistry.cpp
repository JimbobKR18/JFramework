/*
 * ForceRegistry.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: jimmy
 */

#include "ForceRegistry.h"

ForceRegistry::ForceRegistry()
{
}
ForceRegistry::~ForceRegistry()
{
	Clear();
}

/**
 * @brief Apply forces to objects
 */
void ForceRegistry::Update()
{
	for(std::vector<ForceRegistration>::iterator it = mRegistry.begin(); it != mRegistry.end(); ++it)
	{
		it->mGenerator->Update(it->mObject, 0);
	}
}

/**
 * @brief Add force to be apllied to object every frame.
 * @param aObject
 * @param aGenerator
 */
void ForceRegistry::Add(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
	mRegistry.push_back(ForceRegistration(aObject, aGenerator));
}

/**
 * @brief Remove force object pairing
 * @param aObject
 * @param aGenerator
 */
void ForceRegistry::Remove(PhysicsObject *aObject, ForceGenerator *aGenerator)
{
	for(std::vector<ForceRegistration>::iterator it = mRegistry.begin(); it != mRegistry.end(); ++it)
	{
		if(it->mObject == aObject && it->mGenerator == aGenerator)
		{
			mRegistry.erase(it);
			break;
		}
	}
}

/**
 * @brief For an object, remove all forces.
 * @param aObject
 */
void ForceRegistry::RemoveForcesForObject(PhysicsObject *aObject)
{
  for(std::vector<ForceRegistration>::iterator it = mRegistry.begin(); it != mRegistry.end();)
	{
		if(it->mObject == aObject)
    {
      mRegistry.erase(it);
      it = mRegistry.begin();
      continue;
    }
    ++it;
	}
}

/**
 * @brief Clear all forces
 */
void ForceRegistry::Clear()
{
	mRegistry.clear();
}




