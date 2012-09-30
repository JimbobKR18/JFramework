#ifndef __JFramework_Parser_h_
#define __JFramework_Parser_h_

#include "Common.h"

class Parser
{
  private:
    std::string mFilename;

  public:
    Parser(std::string const &aFilename);
    virtual ~Parser();

    std::string GetFilename() const;
};

#endif
