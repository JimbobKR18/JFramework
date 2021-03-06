#ifndef __JFramework_Parser_h_
#define __JFramework_Parser_h_

#include "Common.h"
#include "ParserNode.h"

class Parser
{
private:
  int         mCurrentObjectIndex;
  HashString  mFilename;

protected:
  ParserNode* mDictionary;

public:
  Parser(HashString const &aFilename);
  virtual ~Parser();

  // Getters
  int             GetCurrentObjectIndex() const;
  HashString const&     GetFilename() const;
  ParserNode*           GetBaseRoot() const;

  // Setters
  void            SetCurrentObjectIndex(int const aIndex);

  // Reading
  void                  Reset();
  ParserNode*           Find(HashString const &aElement);
  ParserNode*           Find(HashString const &aRoot, HashString const &aElement);
  ParserNodeContainer   FindAll(HashString const &aRoot, HashString const &aElement);
  ParserNodeContainer   FindAll(HashString const &aElement);
  virtual void    Parse() = 0;

  // Writing
  void            Place(HashString const &aElement, HashString const &aValue);
  void            Place(HashString const &aRoot, HashString const &aElement, HashString const &aValue);
  virtual void    Write() = 0;
  
private:
  ParserNode*           SetUpTree(ParserNode* aBase, std::vector<HashString> const &aStringHierarchy); 
};

#endif
