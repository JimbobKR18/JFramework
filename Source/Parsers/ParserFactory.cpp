#include "ParserFactory.h"
#include "TextParser.h"

std::unordered_map<int, Parser*> ParserFactory::mParserCache;
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
  
  int hash = (aFolder + aFileName).ToHash();
  if(extension == "txt")
  {
    if(mParserCache.find(hash) == mParserCache.end())
    {
      mParserCache[hash] = new TextParser(Common::RelativePath(aFolder, aFileName), MODE_INPUT);
    }
    mParserCache[hash]->Reset();
    return mParserCache[hash];
  }
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

void ParserFactory::Cleanup()
{
  for(std::unordered_map<int, Parser*>::iterator it = mParserCache.begin(); it != mParserCache.end(); ++it)
  {
    delete it->second;
  }
  mParserCache.clear();
}
