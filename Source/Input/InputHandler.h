#ifndef __JFramework__InputHandler__
#define __JFramework__InputHandler__

#include "Common.h"

class DeviceInfo
{
private:
  int mId;
  HashString mName;
  int mAxes[6];
  
public:
  DeviceInfo(int const aId, HashString const &aName);
  
  int const GetId() const;
  HashString const GetName() const;
  int const GetAxis(int const aIndex);
  
  void SetAxis(int const aIndex, int const aValue);
};

class InputHandler
{
protected:
  std::unordered_map<int, DeviceInfo*> mDevices;
  
public:
  typedef std::unordered_map<int, DeviceInfo*> DeviceContainer;
  typedef DeviceContainer::iterator DeviceIT;
  typedef DeviceContainer::const_iterator DeviceConstIT;

  InputHandler();
  virtual ~InputHandler();
  
  virtual void Update() = 0;
  virtual int GetInputCount() = 0;
  virtual void DeviceAdd(int const aId) = 0;
  virtual void DeviceRemove(int const aId) = 0;
  
  DeviceInfo* GetDevice(int const index) const;
  DeviceContainer GetDevices() const;
};

#endif