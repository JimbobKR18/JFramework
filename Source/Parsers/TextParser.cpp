#include "TextParser.h"
#include "Common.h"

Root const *Root::Search(std::string const &aValue) const
{
  // Found our node
  if(mName == aValue)
    return this;
  else
  {
    // Search children for node
    for(std::vector<Root*>::const_iterator it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root const *ret = (*it)->Search(aValue);
      if(ret)
      {
    	  if(ret->mName == aValue)
    		  return ret;
      }
    }
  }

  return NULL;
}

Root::~Root()
{
  // Delete all children associated with this root
  for(std::vector<Root*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
      delete *it;
}

TextParser::TextParser(std::string const &aFilename, bool aAutoParse) : Parser(aFilename)
{
  mDictionary = new Root();
  mInput.open(aFilename.c_str());

  if(mInput.good())
  {
    // If we want to push all objects into a dictionary,
    // set it to autoparse, but for files with values
    // of similar name, that file cannot be autoparsed.
    if(aAutoParse)
      Parse();
  }
  else
    std::cout << "Input file: " << aFilename << " not found!" << std::endl;
}
TextParser::~TextParser()
{
}

bool TextParser::Find(std::string const &aElement)
{
  // Search inside roots to find value
  Root const *value = mDictionary->Search(aElement);

  if(!value)
	  return NULL;

  return value->mName.length() > 0;
}
std::string TextParser::Find(std::string const &aRoot, std::string const &aElement)
{
  // Find node and search it for an element
  Root const *node = mDictionary->Search(aRoot);

  if(!node)
	  return "BadString";

  Root const *value = node->Search(aElement);

  if(!value)
  	  return "BadString";

  return value->mValue;
}
void TextParser::Parse()
{
  Root *mCurNode = NULL;

  while(!mInput.eof())
  {
    Root *node = new Root();
    std::string type;
    mInput >> node->mName;

    // If a closing bracket, the node is done
    if(node->mName == "}")
    {
      delete node;
      if(mCurNode->mParent)
        mCurNode = mCurNode->mParent;
      continue;
    }

    mInput >> type;

    // We found our value
    if(type == "=")
    {
      mInput >> node->mValue;
      node->mParent = mCurNode;
      mCurNode->mChildren.push_back(node);
    }
    // Start a new child node
    else if(type == "{")
    {
      if(mCurNode)
    	  mCurNode->mChildren.push_back(node);

      node->mParent = mCurNode;
      mCurNode = node;
    }
    // Close the node
    else if(type == "}")
    {
      delete node;
      if(mCurNode->mParent)
    	  mCurNode = mCurNode->mParent;
    }
  }

  mDictionary = mCurNode;
}

float TextParser::GetNextFloat(float &rValue)
{
  std::string empty;
  
  // Search for next value, could technically be int,
  // so you better know what you're searching for!
  mInput >> empty;
  while(mInput.good() && ((empty.empty() || empty == "{" || empty == "}") || (!empty.empty() && !isdigit(empty[0]))))
  {
    mInput >> empty;
  }
  rValue = Common::StringToFloat(empty);
  return rValue;
}
int TextParser::GetNextInt(int &rValue)
{
  std::string empty;
  
  // Search for next value, could technically be float,
  // so you better know what you're searching for!
  mInput >> empty;
  while(mInput.good() && ((empty.empty() || empty == "{" || empty == "}") || (!empty.empty() && !isdigit(empty[0]))))
  {
    mInput >> empty;
  }
  rValue = Common::StringToInt(empty);
  return rValue;
}
std::string TextParser::GetNextString(std::string &rValue)
{
  std::string empty;
  
  // Search for next value, finding a string isn't hard
  mInput >> empty;
  while(mInput.good() && (empty.empty() || empty == "{" || empty == "}" || empty == "="))
  {
    mInput >> empty;
  }
  rValue = empty;
  return rValue;
}

bool TextParser::IsGood()
{
  // Helper function to determine if file is read
  return mInput.good();
}
