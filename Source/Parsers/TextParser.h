#ifndef __JFramework_TextParser_h_
#define __JFramework_TextParser_h_

#include <fstream>
#include "Parser.h"

enum TextMode
{
  MODE_INPUT = 0,
  MODE_OUTPUT
};

// Forward declare Root
class Root;

// ... for these
typedef std::set<Root*> RootContainer;
typedef RootContainer::const_iterator rootConstIT;
typedef RootContainer::iterator rootIT;

struct Root
{
  std::string         mValue;
  std::string         mName;
  RootContainer       mChildren;
  Root*               mParent;

  Root();

  Root const *Search(std::string const &aValue) const;
  void Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue);
  ~Root();

  void operator=(Root const &rhs)
  {
	  mValue = rhs.mValue;
	  mName = rhs.mName;
	  mChildren = rhs.mChildren;
	  mParent = rhs.mParent;
  }
private:
  Root *Find(std::string const &aValue);
};

class TextParser : public Parser
{
  private:
    std::ifstream mInput;
    std::ofstream mOutput;
    Root*         mDictionary;
    std::string   mCurrentRoot;
    bool          mAutoParse;
    bool          mWrittenOut;
  public:
    TextParser(std::string const &aFilename, bool aAutoParse = true, TextMode const &aMode = MODE_INPUT);
    ~TextParser();

    // Read
    bool          Find(std::string const &aElement);
    std::string   Find(std::string const &aRoot, std::string const &aElement);
    void          Parse();
    // Write
    void          Place(std::string const &aRoot, std::string const &aValue);
    void          Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue);
    void          Write();
    void          ResetCurrentRoot();
    // Manual
    float         GetNextFloat(float &rValue);
    int           GetNextInt(int &rValue);
    std::string   GetNextString(std::string &rValue);
    bool          IsGood();
  private:
    void          WriteRoot(Root const *aRoot);
    std::string   ParseLiteral(std::string const &aLiteral);
    std::string   ParseNegative(std::string const &aValue);
};

#endif
