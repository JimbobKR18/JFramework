#include "TextParser.h"

Root const *Root::Search(std::string const &aValue) const
{
  if(mName == aValue)
    return this;
  else
  {
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

bool TextParser::Find(std::string const &aElement)
{
  Root const *value = mDictionary->Search(aElement);
  return value->mName.length() > 0;
}
std::string TextParser::Find(std::string const &aRoot, std::string const &aElement)
{
  Root const *node = mDictionary->Search(aRoot);
  Root const *value = node->Search(aElement);
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
