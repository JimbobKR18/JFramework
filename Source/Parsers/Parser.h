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

    // Reading
    virtual bool          Find(std::string const &aElement) = 0;
    virtual std::string   Find(std::string const &aRoot, std::string const &aElement) = 0;
    virtual void          Parse() = 0;

    // Writing
    virtual void          Place(std::string const &aRoot, std::string const &aValue) = 0;
    virtual void          Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue) = 0;
    virtual void          Write() = 0;
};

#endif
