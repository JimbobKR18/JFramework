#include "InputHandler.h"

DeviceInfo::DeviceInfo(int const aId, HashString const &aName) : mId(aId), mName(aName)
{
  for(int i = 0; i < 6; ++i)
    mAxes[i] = 0;
}
  
int const DeviceInfo::GetId() const
{
  return mId;
}

HashString const DeviceInfo::GetName() const
{
  return mName;
}

int const DeviceInfo::GetAxis(int const aIndex)
{
  if(aIndex < 0 || aIndex >= 6)
    assert(!"Requested axis OOB");
    
  return mAxes[aIndex];
}
  
void DeviceInfo::SetAxis(int const aIndex, int const aValue)
{
  if(aIndex < 0 || aIndex >= 6)
    assert(!"Requested axis OOB");
    
  mAxes[aIndex] = aValue;
}
  

InputHandler::InputHandler() : mDevices()
{
}

InputHandler::~InputHandler()
{
  for(DeviceIT it = mDevices.begin(); it != mDevices.end();)
  {
    delete it->second;
  }
  mDevices.clear();
}

DeviceInfo* InputHandler::GetDevice(int const index) const
{
  DeviceConstIT instance = mDevices.find(index);
  if(instance == mDevices.end())
  {
    return nullptr;
  }
  
  return instance->second;
}

InputHandler::DeviceContainer InputHandler::GetDevices() const
{
  return mDevices;
}
