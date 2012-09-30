#include "Parser.h"

Parser::Parser(std::string const &aFilename) : mFilename(aFilename)
{

}
Parser::~Parser()
{

}

std::string Parser::GetFilename() const
{
  return mFilename;
}
