#include "FileCache.h"

FileCache::FileContainer FileCache::mFiles;

FileCache::FileCache()
{
}

FileCache::~FileCache()
{
}

/**
 * @brief Retrieve file from cache, else load it from disk.
 * @param aFilename Name of file to load (full directory and all)
 * @return File contents.
 */
HashString FileCache::GetFile(HashString const &aFilename)
{
  // Keep an eye on this hash function.
  HashType hash = aFilename.ToHash() + aFilename.Length();
  FileContainer::iterator file = mFiles.find(hash);
  if(file != mFiles.end())
  {
#ifdef _DEBUG
    if(aFilename != file->second->GetFileName())
    {
      DebugLogPrint("File %s clashed with %s.\n", aFilename.ToCharArray(), file->second->GetFileName().ToCharArray());
    }
#endif
    return file->second->GetFileContents();
  }
  
  std::ifstream fileFromDisk(aFilename.ToCharArray());
  
  if(!fileFromDisk.good())
  {
    DebugLogPrint("Input file %s not found!", aFilename.ToCharArray());
    assert(!"Input file not found!");
  }
  
  HashString fileContents = std::string((std::istreambuf_iterator<char>(fileFromDisk)), std::istreambuf_iterator<char>());
  mFiles[hash] = new FileCacheData(aFilename, fileContents);
  fileFromDisk.close();
  return fileContents;
}

/**
 * @brief Clear out file cache.
 */
void FileCache::Clear()
{
  for(FileIT it = mFiles.begin(); it != mFiles.end(); ++it)
  {
    delete it->second;
  }
  mFiles.clear();
}
