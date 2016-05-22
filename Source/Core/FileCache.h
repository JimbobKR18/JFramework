#ifndef __JFramework__FileCache_H_
#define __JFramework__FileCache_H_

#include "Common.h"

class FileCache
{
private:
  typedef std::unordered_map<int, HashString> FileContainer;
  static FileContainer mFiles;
public:
  FileCache();
  virtual ~FileCache();
  
  static HashString GetFile(HashString const &aFilename);
};

#endif // __JFramework__FileCache_H_
