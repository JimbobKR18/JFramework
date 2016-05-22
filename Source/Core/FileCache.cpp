#include "FileCache.h"

FileCache::FileContainer FileCache::mFiles;

FileCache::FileCache()
{
}

FileCache::~FileCache()
{
}

HashString FileCache::GetFile(HashString const &aFilename)
{
  // Keep an eye on this hash function.
  int hash = aFilename.ToHash() + aFilename.Length();
  FileContainer::iterator file = mFiles.find(hash);
  if(file != mFiles.end())
  {
#ifdef _DEBUG
    DebugLogPrint("Hash %d clashed with %d, keep an eye out. \n(%s)\n(%s)", hash, file->first, aFilename.ToCharArray(), file->second.ToCharArray());
#endif
    return file->second;
  }
  
  std::ifstream fileFromDisk(aFilename.ToCharArray());
  
  if(!fileFromDisk.good())
  {
    DebugLogPrint("Input file %s not found!", aFilename.ToCharArray());
    assert(!"Input file not found!");
  }
  
  HashString fileContents = std::string((std::istreambuf_iterator<char>(fileFromDisk)), std::istreambuf_iterator<char>());
  mFiles[hash] = fileContents;
  fileFromDisk.close();
  return fileContents;
}