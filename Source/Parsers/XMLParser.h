#ifndef __JFramework_XMLParser_h_
#define __JFramework_XMLParser_h_

#include "Parser.h"
#include "TinyXML/tinyxml2.h"

class XMLParser : public Parser
{
private:
  tinyxml2::XMLDocument mDoc;
  tinyxml2::XMLPrinter  mPrinter;
public:
  XMLParser(std::string const &aFileName);
  ~XMLParser();

  // Read
  bool          Find(std::string const &aElement);
  std::string   Find(std::string const &aRoot, std::string const &aElement);
  void          Parse();

  // Write
  void          Place(std::string const &aRoot, std::string const &aValue);
  void          Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue);
  void          Write();
};

#endif
