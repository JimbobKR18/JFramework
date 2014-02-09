#include "XMLParser.h"

XMLParser::XMLParser(std::string const &aFileName) : Parser(aFileName)
{
  Parse();
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
  mDoc.LoadFile(GetFilename().c_str());
}

void XMLParser::Place(std::string const &aRoot, std::string const &aValue)
{
  mPrinter.OpenElement(aRoot.c_str());
  mPrinter.PushAttribute(aRoot.c_str(), aValue.c_str());
  mPrinter.CloseElement();
}

void XMLParser::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
{
  mPrinter.OpenElement(aRoot.c_str());
  mPrinter.PushAttribute(aElement.c_str(), aValue.c_str());
  mPrinter.CloseElement();
}

void XMLParser::Write()
{
  mDoc.Print(&mPrinter);
  mDoc.SaveFile(GetFilename().c_str());
}
