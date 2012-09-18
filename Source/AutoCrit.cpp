//
//  AutoCrit.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#include "AutoCrit.h"
#include <assert.h>

Thread::Thread(void*(*func)(void *), void *args)
{
	pthread_create(&mThread, NULL, func, args);
}

Thread::Thread(Thread const &rhs)
{
}

Thread::~Thread()
{
	pthread_exit(&mThread);
}

CriticalSection::CriticalSection()
{
}

CriticalSection::~CriticalSection()
{
  pthread_mutex_destroy(&mMutex);
}

void CriticalSection::Init()
{
  pthread_mutex_init(&mMutex, NULL);
}

bool CriticalSection::EnterCriticalSection(int aTimeout)
{
  if(aTimeout == kDurationForever)
	{
		pthread_mutex_lock(&mMutex);
		return true;
	}
	else if(aTimeout == kDurationImmediate)
	{
		int ret = pthread_mutex_trylock(&mMutex);
		return ret ? false : true;
	}
	else
	{
		return false;
	}
}

bool CriticalSection::ExitCriticalSection()
{
  pthread_mutex_unlock(&mMutex);
  return true;
}

AutoCrit::AutoCrit()
{
  // Can't create a blank AutoCrit
  assert(0);
}

AutoCrit::AutoCrit(AutoCrit const &aAutoCrit, bool aBlock) : mCriticalSection(aAutoCrit.mCriticalSection)
{
  mCriticalSection.Init();
  mCriticalSection.EnterCriticalSection(aBlock ? kDurationForever : kDurationImmediate);
}

AutoCrit::AutoCrit(CriticalSection const &aCriticalSection, bool aBlock) : mCriticalSection(aCriticalSection)
{
  mCriticalSection.Init();
  mCriticalSection.EnterCriticalSection(aBlock ? kDurationForever : kDurationImmediate);
}

AutoCrit::~AutoCrit()
{
  mCriticalSection.ExitCriticalSection();
}
