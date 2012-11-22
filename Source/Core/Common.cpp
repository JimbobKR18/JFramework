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
#ifdef _WIN32
  std::string ret;
#else
  std::string ret = "../Assets/";
#endif
  ret.append(aFileName);
  return ret;
}

#ifndef _WIN32
long GetTickCount()
{
	return timeGetTime();
}

long timeGetTime()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec * 1000) + (now.tv_nsec / (1000 * 1000));
}
#endif

int StringToInt(std::string const &value)
{
	int ret = atoi(value.c_str());
	return ret;
}

