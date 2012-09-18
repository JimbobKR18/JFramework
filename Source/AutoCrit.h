//
//  AutoCrit.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef __JFramework__AutoCrit__
#define __JFramework__AutoCrit__

#include <pthread.h>

int const kDurationForever = 0xffffffff;
int const kDurationImmediate = 0;

class Thread
{
private:
	pthread_t mThread;
public:
	Thread(void*(*func)(void *), void *args);
	Thread(Thread const &rhs);
	~Thread();
};

class CriticalSection
{
private:
  pthread_mutex_t mMutex;
  
public:
  CriticalSection();
  ~CriticalSection();
  
  void Init();
  
  bool EnterCriticalSection(int aTimeout);
  bool ExitCriticalSection();
};

class AutoCrit
{
private:
  CriticalSection mCriticalSection;
  
public:
  AutoCrit();
  AutoCrit(AutoCrit const &aCrit, bool aBlock = true);
  AutoCrit(CriticalSection const &aCriticalSection, bool aBlock = true);
  ~AutoCrit();
};

#endif /* defined(__JFramework__AutoCrit__) */
