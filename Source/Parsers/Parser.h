#ifndef __JFramework_Parser_h_
#define __JFramework_Parser_h_

#include "Common.h"
#include "Root.h"

class Parser
{
  private:
    int         mCurrentObjectIndex;
    HashString  mFilename;

  protected:
    Root* mDictionary;

  public:
    Parser(HashString const &aFilename);
    virtual ~Parser();

    // Getters
    int             GetCurrentObjectIndex() const;
    std::string     GetFilename() const;
    Root*           GetBaseRoot() const;

    // Setters
    void            SetCurrentObjectIndex(int const aIndex);

    // Reading
    Root*           Find(HashString const &aElement);
    Root*           Find(HashString const &aRoot, HashString const &aElement);
    std::set<Root*> FindAll(HashString const &aElement);
    virtual void    Parse() = 0;

    // Writing
    void            Place(HashString const &aElement, HashString const &aValue);
    void            Place(HashString const &aRoot, HashString const &aElement, HashString const &aValue);
    virtual void    Write() = 0;
    
  private:
    Root*           SetUpTree(Root* aBase, std::vector<HashString> const &aStringHierarchy); 
};

#endif
