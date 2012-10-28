#ifndef __JFramework_TextParser_h_
#define __JFramework_TextParser_h_

#include <fstream>
#include "Parser.h"

struct Root
{
  std::string mValue;
  std::string mName;
  std::vector<Root*> mChildren;
  Root *mParent;

  Root const *Search(std::string const &aValue) const;
  ~Root();

  void operator=(Root const &rhs)
  {
	  mValue = rhs.mValue;
	  mName = rhs.mName;
	  mChildren = rhs.mChildren;
	  mParent = rhs.mParent;
  }
};

class TextParser : public Parser
{
  private:
    std::ifstream mInput;
    Root *mDictionary;
  public:
    TextParser(std::string const &aFilename);
    ~TextParser();

    bool Find(std::string const &aElement);
    std::string Find(std::string const &aRoot, std::string const &aElement);
    void Parse();
};

#endif
