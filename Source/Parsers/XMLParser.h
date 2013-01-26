#ifndef __JFramework_XMLParser_h_
#define __JFramework_XMLParser_h_

#include "Parser.h"
#include "TinyXML/tinyxml2.h"

class XMLParser : public Parser
{
private:
  tinyxml2::XMLDocument mDoc;
public:
  XMLParser(std::string const &aFilename);
  ~XMLParser();

  bool          Find(std::string const &aElement);
  std::string   Find(std::string const &aRoot, std::string const &aElement);
  void          Parse();
};

#endif
