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
#include "InputHandler.h"

struct InputInfo
{
  HashString  mInput;
  Vector3     mLocation;
  int         mId;

  InputInfo(HashString const &aInput, Vector3 const &aLocation, int const aId) : mInput(aInput), mLocation(aLocation), mId(aId) {}
  bool operator<(InputInfo const &aRhs) const {return mInput < aRhs.mInput;}
};

class InputManager : public Manager
{
private:
  InputHandler*       mHandler;
  std::set<InputInfo> mInputs;
  bool                mAcceptInput;

  static unsigned const sUID;
public:
  InputManager(GameApp *aApp);
  ~InputManager();
  
  void                AddInput(HashString const &aInput, Vector3 const &aLocation, int const aId = 0);
  void                RemoveInput(HashString const &aInput, int const aId = 0);
  void                ClearInputs();
  
  void                AcceptInputs();
  void                DeclineInputs();
  bool                AcceptingInputs();
  
  InputHandler*       GetInputHandler() const;

  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "InputManager";}
  static unsigned     GetUID() {return sUID;}
  
public:
  typedef std::set<InputInfo>::iterator InputIT;
};

#endif /* defined(__JFramework__InputManager__) */
