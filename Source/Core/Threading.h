//
//  Threading.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef __JFramework__Threading__
#define __JFramework__Threading__

#include <thread>
#include <mutex>
#include <functional>
#include <chrono>

#define Reference std::ref
typedef std::thread Thread;
typedef std::mutex Mutex;

namespace Threading
{
  void thread_sleep(unsigned int aMillis);
}

#endif /* defined(__JFramework__Threading__) */
