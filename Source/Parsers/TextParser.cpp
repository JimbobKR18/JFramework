#include "TextParser.h"
#include "Common.h"

Root::Root() : mValue(""), mName(""), mChildren(), mParent(NULL)
{
}

Root const *Root::Search(std::string const &aValue) const
{
  if(mName == aValue)
    return this;
  else
  {
    // Search children for node
    for(rootConstIT it = mChildren.begin(); it != mChildren.end(); ++it)
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

void Root::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
{
  if(mName == aRoot)
  {
    Root *node = Find(aElement);

    if(!node)
    {
      node = new Root();
      node->mName = aElement;
    }
    node->mValue = aValue;
    node->mParent = this;
    mChildren.push_back(node);
    return;
  }
  else
  {
    if(mName == aElement)
    {
      mValue = aValue;
    }
    else
    {
      for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
      {
        (*it)->Place(aRoot, aElement, aValue);
      }
    }
  }
}

Root *Root::Find(std::string const &aValue)
{
  // Found our node
  if(mName == aValue)
    return this;
  else
  {
    // Search children for node
    for(rootIT it = mChildren.begin(); it != mChildren.end(); ++it)
    {
      Root *ret = (*it)->Find(aValue);
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

TextParser::TextParser(std::string const &aFilename, bool aAutoParse, TextMode const &aMode) : Parser(aFilename),
                                                                                               mCurrentRoot(""),
                                                                                               mAutoParse(aAutoParse)
{
  mDictionary = new Root();

  if(aMode == MODE_INPUT)
    mInput.open(aFilename.c_str());
  else
    mOutput.open(aFilename.c_str());

  if(mInput.good())
  {
    // If we want to push all objects into a dictionary,
    // set it to autoparse, but for files with values
    // of similar name, that file cannot be autoparsed.
    if(aMode == MODE_INPUT && mAutoParse)
      Parse();
  }
  else
  {
    std::cout << "Input file: " << aFilename << " not found!" << std::endl;
    assert(0);
  }
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
      std::string value;
      mInput >> value;
      node->mValue = ParseLiteral(value);
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

void TextParser::Place(std::string const &aRoot, std::string const &aValue)
{
  if(mAutoParse)
    mDictionary->Place(mDictionary->mName, aRoot, aValue);
  else
  {
    if(aValue == "")
    {
      bool isTextFile = false;
      bool newRoot = aRoot != mCurrentRoot;
      bool empty = mCurrentRoot.empty();

      mCurrentRoot = aRoot;

      //if(mCurrentRoot.length() > 5)
        //isTextFile = mCurrentRoot.substr(mCurrentRoot.length() - 4, 4) == ".txt";
      if(newRoot && !empty && !isTextFile)
        mOutput << "}" << std::endl;

      mOutput << aRoot << std::endl;

      if(!isTextFile)
        mOutput << "{" << std::endl;
    }
    else
    {
      mOutput << aRoot << " = " <<  aValue << std::endl;
    }
  }
}

void TextParser::Place(std::string const &aRoot, std::string const &aElement, std::string const &aValue)
{
  if(mAutoParse)
    mDictionary->Place(aRoot, aElement, aValue);
  else
  {
    if(aValue == "")
    {
      bool isTextFile = false;
      bool newRoot = aRoot != mCurrentRoot;
      bool empty = mCurrentRoot.empty();

      mCurrentRoot = aRoot;

      //if(mCurrentRoot.length() > 5)
        //isTextFile = mCurrentRoot.substr(mCurrentRoot.length() - 4, 4) == ".txt";

      if(newRoot && !empty && !isTextFile)
        mOutput << "}" << std::endl;
      mOutput << aRoot << std::endl;

      if(!isTextFile)
        mOutput << "{" << std::endl;
    }
    else
    {
      mOutput << aElement << " = " <<  aValue << std::endl;
    }
  }
}

void TextParser::Write()
{
  if(mAutoParse)
    WriteRoot(mDictionary);
  else
    mOutput << "}" << std::endl;
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
  rValue = ParseLiteral(empty);
  return rValue;
}

bool TextParser::IsGood()
{
  // Helper function to determine if file is read
  return mInput.good();
}

void TextParser::WriteRoot(Root const *aRoot)
{
  if(aRoot->mValue == "")
  {
    rootConstIT end = aRoot->mChildren.end();
    mOutput << aRoot->mName << std::endl;

    if(aRoot->mChildren.empty())
      return;

    mOutput << "{" << std::endl;
    for(rootConstIT it = aRoot->mChildren.begin(); it != end; ++it)
    {
      WriteRoot(*it);
    }
    mOutput << "}" << std::endl;
  }
  else
  {
    mOutput << aRoot->mName << " = " << aRoot->mValue << std::endl;
  }
}

std::string TextParser::ParseLiteral(std::string const &aLiteral)
{
  std::string ret;
  if(aLiteral.find("Literal") == std::string::npos)
    ret = aLiteral;
  else
  {
    // Getting the full string
    unsigned pos = aLiteral.find("Literal") + 8;
    char next;
    bool earlyout = false;
    // Literal(blah) is one whole word, extract
    while(pos < aLiteral.length())
    {
      char next = aLiteral[pos];
      if(next == '\n')
      {
        earlyout = true;
        break;
      }
      if(next != ')' && next != '(')
        ret.push_back(next);
      ++pos;
    }
    // Didn't hit newline, get rest of sentence
    while(!earlyout && mInput.get(next))
    {
      if(next == '\n')
        break;
      if(next != ')' && next != '(')
        ret.push_back(next);
    }
  }
  return ret;
}
