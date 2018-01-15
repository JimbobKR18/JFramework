#include "ParserFactory.h"
#include "TextParser.h"

ParserFactory::ParserFactory()
{
}

ParserFactory::~ParserFactory()
{
}

Parser* ParserFactory::CreateInputParser(HashString const &aFolder, HashString const &aFileName)
{
  std::vector<HashString> split = aFileName.Split(".");
  HashString extension = split[split.size() - 1];
  
  if(extension == "txt")
    return new TextParser(Common::RelativePath(aFolder, aFileName), MODE_INPUT);
  else
  {
    DebugLogPrint("Invalid file extension %s passed into ParserFactory.", extension.ToCharArray());
    assert(!"Invalid file extension passed into ParserFactory.");
    return nullptr;
  }
}

Parser* ParserFactory::CreateOutputParser(HashString const &aFolder, HashString const &aFileName)
{
  std::vector<HashString> split = aFileName.Split(".");
  HashString extension = split[split.size() - 1];
  
  if(extension == "txt")
    return new TextParser(Common::RelativePath(aFolder, aFileName), MODE_OUTPUT);
  else
  {
    DebugLogPrint("Invalid file extension %s passed into ParserFactory.", extension.ToCharArray());
    assert(!"Invalid file extension passed into ParserFactory.");
    return nullptr;
  }
}