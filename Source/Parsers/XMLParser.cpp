#include "XMLParser.h"

XMLParser::XMLParser(std::string const &aFileName) : Parser(aFileName)
{
  mDoc.LoadFile(aFileName.c_str());
}

XMLParser::~XMLParser()
{
}

bool XMLParser::Find(std::string const &aElement)
{
  tinyxml2::XMLText *text = mDoc.FirstChildElement(aElement.c_str())->ToText();
  std::string ret = text->Value();
  return ret.length() > 0;
}

std::string XMLParser::Find(std::string const &aRoot, std::string const &aElement)
{
  tinyxml2::XMLText *text = mDoc.FirstChildElement(aRoot.c_str())->FirstChildElement(aElement.c_str())->ToText();
  std::string ret = text->Value();
  return ret;
}

void XMLParser::Parse()
{
  // TODO?
}

void XMLParser::Place(std::string const &aRoot, std::string const &aValue)
{
  // TODO
}

void XMLParser::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
{
  // TODO
}

void XMLParser::Write()
{
  //mDoc.Save(GetFileName().c_str());
}
