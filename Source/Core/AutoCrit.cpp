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
#ifndef _WIN32
	pthread_create(&mThread, NULL, func, args);
#endif
}

Thread::Thread(Thread const &rhs)
{
#ifndef _WIN32
	mThread = rhs.mThread;
#endif
}

Thread::~Thread()
{
#ifndef _WIN32
	pthread_exit(&mThread);
#endif
}

CriticalSection::CriticalSection()
{
}

CriticalSection::~CriticalSection()
{
#ifndef _WIN32
  pthread_mutex_destroy(&mMutex);
#endif
}

void CriticalSection::Init()
{
#ifndef _WIN32
  pthread_mutex_init(&mMutex, NULL);
#endif
}

bool CriticalSection::EnterCriticalSection(int aTimeout)
{
#ifndef _WIN32
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
#else
	return false;
#endif
}

bool CriticalSection::ExitCriticalSection()
{
#ifndef _WIN32
  pthread_mutex_unlock(&mMutex);
#endif
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
