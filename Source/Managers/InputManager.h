//
//  InputManager.h
//  JFramework
//
//  Created by Jimmy Spencer on 12/27/12.
//
//

#ifndef __JFramework__InputManager__
#define __JFramework__InputManager__

#include "Manager.h"
#include "MathExt.h"

struct InputInfo
{
  std::string mInput;
  Vector3     mLocation;

  InputInfo(std::string const &aInput, Vector3 const &aLocation) : mInput(aInput), mLocation(aLocation) {}
  bool operator<(InputInfo const &aRhs) const {return mInput < aRhs.mInput;}
};

class InputManager : public Manager
{
private:
  std::set<InputInfo> mInputs;
public:
  InputManager(GameApp *aApp);
  ~InputManager();
  
  void                AddInput(std::string const &aInput, Vector3 const &aLocation);
  void                RemoveInput(std::string const &aInput);
  void                ClearInputs();
  
  // Derived from Manager
  void                Update();
	void                SendMessage(Message const &aMessage);
	static std::string  GetName() {return "InputManager";}
  
public:
  typedef std::set<InputInfo>::iterator InputIT;
};

#endif /* defined(__JFramework__InputManager__) */
