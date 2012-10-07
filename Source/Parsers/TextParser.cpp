#include "TextParser.h"

Root Root::Search(std::string const &aValue) const
{
  if(mName == aValue)
    return *this;
  else
  {
    for(std::vector<Root*>::const_iterator it = mChildren.begin(); it != mChildren.end(); ++it)
      return (*it)->Search(aValue);
  }

  return Root();
}

Root::~Root()
{
  for(std::vector<Root*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
      delete *it;
}

TextParser::TextParser(std::string const &aFilename) : Parser(aFilename)
{
  mDictionary = new Root();
  mInput.open(aFilename.c_str());

  if(mInput.good())
    Parse();
}
TextParser::~TextParser()
{

}

std::string TextParser::Find(std::string const &aElement)
{
  Root value = mDictionary->Search(aElement);
  return value.mValue;
}
std::string TextParser::Find(std::string const &aRoot, std::string const &aElement)
{
  Root node = mDictionary->Search(aRoot);
  Root value = node.Search(aElement);
  return value.mValue;
}
void TextParser::Parse()
{
  Root *mCurNode = NULL;

  while(!mInput.eof())
  {
    Root *node = new Root();
    std::string type;
    mInput >> node->mName;

    if(node->mName == "}")
	{
	  delete node;
	  if(mCurNode->mParent)
		  mCurNode = mCurNode->mParent;
	  continue;
	}

    mInput >> type;

    if(type == "=")
    {
      mInput >> node->mValue;
      node->mParent = mCurNode;
      mCurNode->mChildren.push_back(node);
    }
    else if(type == "{")
    {
      if(mCurNode)
    	  mCurNode->mChildren.push_back(node);

      node->mParent = mCurNode;
      mCurNode = node;
    }
    else if(type == "}")
    {
      delete node;
      if(mCurNode->mParent)
    	  mCurNode = mCurNode->mParent;
    }
  }

  mDictionary = mCurNode;
}
