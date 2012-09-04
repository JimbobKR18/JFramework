//
//  LUAObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/3/12.
//
//

#include "LUAObject.h"

LUAObject::LUAObject()
{
  mState = luaL_newstate();
  luaL_openlibs(mState);
}

LUAObject::LUAObject(std::string const &aFilename) : mFilename(aFilename)
{
  mState = luaL_newstate();
  luaL_openlibs(mState);
  luaL_dofile(mState, mFilename.c_str());
}

LUAObject::LUAObject(LUAObject const &aObject) : mFilename(aObject.mFilename)
{
  mState = luaL_newstate();
  luaL_openlibs(mState);
  luaL_dofile(mState, mFilename.c_str());
}

LUAObject::~LUAObject()
{
  lua_close(mState);
}

void LUAObject::SetFilename(std::string const &aFilename)
{
  mFilename = aFilename;
}

void LUAObject::Execute()
{
  luaL_dofile(mState, mFilename.c_str());
}