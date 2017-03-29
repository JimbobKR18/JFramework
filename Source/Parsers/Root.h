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
class Root;

// ... for these
typedef std::set<Root*> RootContainer;
typedef RootContainer::const_iterator rootConstIT;
typedef RootContainer::iterator rootIT;

class Root
{
private:
  HashString          mValue;
  HashString          mName;
  RootContainer       mChildren;
  Root*               mParent;
  bool                mTouched;

public:
  Root();
  virtual ~Root();

  Root const*    Search(HashString const &aValue) const;
  Root*          Find(HashString const &aValue);
  void           Place(HashString const &aRoot, HashString const &aElement, HashString const &aValue);
  void           Insert(Root* root);

  // GETTERS
  HashString     GetName() const;
  HashString     GetValue() const;
  Root*          GetParent() const;
  RootContainer& GetChildren();
  RootContainer  GetUntouchedRoots() const;

  // SETTERS
  void           SetName(HashString const &aName);
  void           SetValue(HashString const &aValue);
  void           SetParent(Root* aParent);

  // OPERATIONS
  void           operator=(Root const &rhs);
  bool           operator<(Root const &rhs);
};

#endif /* ROOT_H_ */
