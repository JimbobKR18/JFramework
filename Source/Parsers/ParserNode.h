/*
 * Root.h
 *
 *  Created on: Sep 4, 2014
 *      Author: jimbob
 */

#ifndef ROOT_H_
#define ROOT_H_

#include <string>
#include <set>
#include "Common.h"

// Forward declare Root
class ParserNode;

// ... for these
typedef std::set<ParserNode*> ParserNodeContainer;
typedef ParserNodeContainer::const_iterator parserNodeConstIT;
typedef ParserNodeContainer::iterator parserNodeIT;

class ParserNode
{
private:
  HashString          mValue;
  HashString          mName;
  ParserNodeContainer       mChildren;
  ParserNode*               mParent;
  bool                mTouched;

public:
  ParserNode();
  virtual ~ParserNode();

  ParserNode const*     Search(HashString const &aValue) const;
  ParserNode*           Find(HashString const &aValue);
  std::set<ParserNode*> FindAll(HashString const &aValue);
  void            Place(HashString const &aElement, HashString const &aValue);
  void            Insert(ParserNode* root);

  // GETTERS
  HashString     GetName() const;
  HashString     GetValue() const;
  ParserNode*          GetParent() const;
  ParserNodeContainer& GetChildren();
  ParserNodeContainer  GetUntouchedRoots() const;

  // SETTERS
  void           SetName(HashString const &aName);
  void           SetValue(HashString const &aValue);
  void           SetParent(ParserNode* aParent);

  // OPERATIONS
  void           operator=(ParserNode const &rhs);
  bool           operator<(ParserNode const &rhs);
};

#endif /* ROOT_H_ */
