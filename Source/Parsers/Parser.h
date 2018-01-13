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
  std::string     GetFilename() const;
  ParserNode*           GetBaseRoot() const;

  // Setters
  void            SetCurrentObjectIndex(int const aIndex);

  // Reading
  ParserNode*           Find(HashString const &aElement);
  ParserNode*           Find(HashString const &aRoot, HashString const &aElement);
  std::set<ParserNode*> FindAll(HashString const &aElement);
  virtual void    Parse() = 0;

  // Writing
  void            Place(HashString const &aElement, HashString const &aValue);
  void            Place(HashString const &aRoot, HashString const &aElement, HashString const &aValue);
  virtual void    Write() = 0;
  
private:
  ParserNode*           SetUpTree(ParserNode* aBase, std::vector<HashString> const &aStringHierarchy); 
};

#endif
