#ifndef __JFramework_ParserFactory_h_
#define __JFramework_ParserFactory_h_

#include "Common.h"
#include "Parser.h"

class ParserFactory
{
private:
  static std::unordered_map<HashType, Parser*> mParserCache;
  ParserFactory();
  
public:
  virtual ~ParserFactory();
  
  static Parser* CreateInputParser(HashString const &aFolder, HashString const &aFileName);
  static Parser* CreateOutputParser(HashString const &aFolder, HashString const &aFileName);
  static void Cleanup();
};

#endif // __JFramework_ParserFactory_h_
