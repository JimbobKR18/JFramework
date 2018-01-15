#ifndef __JFramework_GameObject_H_
#define __JFramework_GameObject_H_

#include "GameObject.h"
#include "Manager.h"
#include "Parser.h"
#include "GameObjectFactory.h"

class ObjectManager : public Manager
{
private:
#ifdef _DEBUG
  std::map<size_t, int> mDelayedDeletions;
#endif
  std::vector<GameObject*> mObjects;
  std::vector<GameObject*> mStaticObjects;
  std::set<GameObject*> mAllocatedObjects;
  
  GameObjectFactory *mFactory;

  static unsigned const sUID;
public:
  ObjectManager(GameApp* aApp);
  virtual ~ObjectManager();
  
  // Getters
  GameObjectFactory*  GetGameObjectFactory() const;
  std::set<GameObject*>& GetAllocatedObjects();
  
  // Setters
  void                SetGameObjectFactory(GameObjectFactory* aFactory);

  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "ObjectManager";}
  static unsigned     GetUID() {return sUID;}

  GameObject*         CreateObject(HashString const &aFilename, HashString const &aFolder = "Game", HashString const &aType = "Default");
  GameObject*         CreateObjectNoAdd(HashString const &aFilename, HashString const &aFolder = "Game", HashString const &aType = "Default");
  void                ParseObject(GameObject *aObject, HashString const &aFolder = "Game");
  void                DeleteObject(GameObject *aObj);
  void                ClearObjects();
  void                AddObject(GameObject *aObj, bool aStatic = false);
  void                RemoveObject(GameObject *aObj);
  static void         SerializeLUA();
private:
  void                ParseDictionary(GameObject *aObject, Parser *aParser);
  void                ParseEffects(GameObject *aObject, ParserNode *aEffects);
  typedef std::vector<GameObject*>::iterator ObjectIT;
};

#endif
