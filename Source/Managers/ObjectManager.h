#ifndef __JFramework_GameObject_H_
#define __JFramework_GameObject_H_

#include "GameObject.h"
#include "Manager.h"
#include "TextParser.h"

class ObjectManager : public Manager
{
private:
  std::vector<GameObject*> mObjects;
  std::vector<GameObject*> mStaticObjects;

  static unsigned const sUID;
public:
  ObjectManager(GameApp* aApp);
  virtual ~ObjectManager();

  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "ObjectManager";}
  static unsigned     GetUID() {return sUID;}

  GameObject*         CreateObject(HashString const &aFilename, HashString const &aFolder = "Game");
  GameObject*         CreateObjectNoAdd(HashString const &aFilename, HashString const &aFolder = "Game");
  void                ParseObject(GameObject *aObject);
  void                DeleteObject(GameObject *aObj);
  void                ClearObjects();
  void                AddObject(GameObject *aObj, bool aStatic = false);
  void                RemoveObject(GameObject *aObj);
  static void         SerializeLUA();
private:
  void                ParseDictionary(GameObject *aObject, Parser &aParser);
  typedef std::vector<GameObject*>::iterator ObjectIT;
};

#endif
