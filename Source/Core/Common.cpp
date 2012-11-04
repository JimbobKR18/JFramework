//
//  Common.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/8/12.
//
//

#include "Common.h"

std::string const RelativePath(std::string const &aFileName)
{
#if !defined(ANDROID) && !defined(IOS)
  std::string ret = "../Assets/";
#endif
  ret.append(aFileName);
  return ret;
}

float GetTickCount()
{
#ifdef _WIN32
	return 0;
#else
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec + (now.tv_nsec / (1000.0f * 1000.0f * 1000.0f));
#endif
}

float timeGetTime()
{
	return GetTickCount();
}

int StringToInt(std::string const &value)
{
	int ret = atoi(value.c_str());
	return ret;
}

