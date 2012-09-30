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
  std::string ret = "LUAFiles/";
  ret.append(aFileName);
  return ret;
}

long GetTickCount()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec * 1000000000LL + now.tv_nsec;
}

unsigned timeGetTime()
{
	return GetTickCount();
}

