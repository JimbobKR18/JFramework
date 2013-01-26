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

class InputManager : public Manager
{
private:
  std::set<std::string> mInputs;
public:
  InputManager(GameApp *aApp);
  ~InputManager();
  
  void                AddInput(std::string const &aInput);
  void                RemoveInput(std::string const &aInput);
  
  // Derived from Manager
  void                Update();
	void                SendMessage(Message const &aMessage);
	static std::string  GetName() {return "InputManager";}
  
public:
  typedef std::set<std::string>::iterator InputIT;
};

#endif /* defined(__JFramework__InputManager__) */
