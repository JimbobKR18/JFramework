#ifndef __JFramework_TextParser_h_
#define __JFramework_TextParser_h_

#include <fstream>
#include "Parser.h"

enum TextMode
{
  MODE_INPUT = 0,
  MODE_OUTPUT
};

struct Root
{
  std::string         mValue;
  std::string         mName;
  std::vector<Root*>  mChildren;
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

typedef std::vector<Root*>::const_iterator rootConstIT;
typedef std::vector<Root*>::iterator rootIT;

class TextParser : public Parser
{
  private:
    std::ifstream mInput;
    std::ofstream mOutput;
    Root*         mDictionary;
    std::string   mCurrentRoot;
    bool          mAutoParse;
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
    // Manual
    float         GetNextFloat(float &rValue);
    int           GetNextInt(int &rValue);
    std::string   GetNextString(std::string &rValue);
    bool          IsGood();
  private:
    void          WriteRoot(Root const *aRoot);
};

#endif
