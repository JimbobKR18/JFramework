#ifndef __JFramework__FileCache_H_
#define __JFramework__FileCache_H_

#include "Common.h"

class FileCacheData
{
private:
  HashString mFileName;
  HashString mFileContents;
  
public:
  FileCacheData(HashString const &aFileName, HashString const &aFileContents) : mFileName(aFileName), mFileContents(aFileContents)
  {
  }
  
  HashString const &GetFileName() { return mFileName; }
  HashString const &GetFileContents() { return mFileContents; }
};

class FileCache
{
private:
  typedef std::unordered_map<int, FileCacheData*> FileContainer;
  typedef FileContainer::iterator FileIT;
  static FileContainer mFiles;
public:
  FileCache();
  virtual ~FileCache();
  
  static HashString GetFile(HashString const &aFilename);
  static void Clear();
};

#endif // __JFramework__FileCache_H_
