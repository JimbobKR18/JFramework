//
//  LUAObject.cpp
//  JFramework
//
//  Created by Jimmy Spencer on 9/3/12.
//
//

#include "LUAObject.h"

LUAObject::LUAObject() : Component("LUAObject")
{
  mState = luaL_newstate();
  luaL_openlibs(mState);
}

LUAObject::LUAObject(std::string const &aFilename) : Component("LUAObject"), mFilename(aFilename)
{
  mState = luaL_newstate();
  luaL_openlibs(mState);
  luaL_dofile(mState, mFilename.c_str());
}

LUAObject::LUAObject(LUAObject const &aObject) : Component("LUAObject"), mFilename(aObject.mFilename)
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

void LUAObject::CallFunction(std::string const &aFuncName)
{
  int error = 0;

  if((error = lua_pcall(mState, 0, LUA_MULTRET, 0)) == 0)
  {
    lua_pushstring(mState, aFuncName.c_str());
    lua_pcall(mState, 0, 0, 0);
  }
}

void LUAObject::Update()
{

}

void LUAObject::SendMessage(Message const &aMessage)
{

}

void LUAObject::ReceiveMessage(Message const &aMessage)
{

}
