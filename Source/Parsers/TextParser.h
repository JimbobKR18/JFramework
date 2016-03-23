#ifndef __JFramework_TextParser_h_
#define __JFramework_TextParser_h_

#include <fstream>
#include "Parser.h"

enum TextMode
{
  MODE_INPUT = 0,
  MODE_OUTPUT
};

class TextParser : public Parser
{
  private:
    std::ifstream mInput;
    std::ofstream mOutput;
    bool          mWrittenOut;
    TextMode      mMode;
    int           mCurrentIndent;
  public:
    TextParser(std::string const &aFilename, TextMode const &aMode = MODE_INPUT);
    ~TextParser();

    // Read
    void          Parse();
    // Write
    void          Write();
  private:
    void          WriteRoot(Root *aRoot);
    std::string   InsertIndents();
    std::string   ParseNegative(std::string const &aValue);
};

#endif
