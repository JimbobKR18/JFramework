#include "Threading.h"

namespace Threading
{
  void thread_sleep(unsigned int aMillis)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(aMillis));
  }
}