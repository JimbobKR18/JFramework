#ifndef __JFramework_Parser_h_
#define __JFramework_Parser_h_

#include "Common.h"
#include "Root.h"

class Parser
{
  private:
    int         mCurrentObjectIndex;
    std::string mFilename;

  protected:
    Root* mDictionary;

  public:
    Parser(std::string const &aFilename);
    virtual ~Parser();

    // Getters
    int           GetCurrentObjectIndex() const;
    std::string   GetFilename() const;
    Root*         GetBaseRoot() const;

    // Setters
    void          SetCurrentObjectIndex(int const aIndex);

    // Reading
    Root*         Find(std::string const &aElement);
    Root*         Find(std::string const &aRoot, std::string const &aElement);
    virtual void  Parse() = 0;

    // Writing
    void          Place(std::string const &aElement, std::string const &aValue);
    void          Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue);
    virtual void  Write() = 0;
    
  private:
    Root*         SetUpTree(Root* aBase, std::vector<HashString> const &aStringHierarchy); 
};

#endif
