//
//  Common.h
//  JFramework
//
//  Created by Jimmy Spencer on 9/1/12.
//
//

#ifndef JFramework_Common_h
#define JFramework_Common_h

#include <vector>
#include <string>
#include <cassert>
#include <cstdlib>

std::string const RelativePath(std::string const &aFileName);

#ifndef _WIN32
long GetTickCount();
long timeGetTime();
#endif

int StringToInt(std::string const &value);

#endif
