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

    std::string           GetFilename() const;
    virtual bool          Find(std::string const &aElement) = 0;
    virtual std::string   Find(std::string const &aRoot, std::string const &aElement) = 0;
    virtual void          Parse() = 0;
};

#endif
