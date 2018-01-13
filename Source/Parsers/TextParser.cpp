#include "TextParser.h"
#include "FileCache.h"

bool RootSortPredicate(ParserNode* aRoot1, ParserNode* aRoot2)
{
  return (*aRoot1) < (*aRoot2);
}

TextParser::TextParser(HashString const &aFilename, TextMode const &aMode) : Parser(aFilename),
                                                                              mInput(),
                                                                              mOutput(),
                                                                              mWrittenOut(false),
                                                                              mMode(aMode),
                                                                              mCurrentIndent(0)
{
  if(aMode == MODE_INPUT)
    mInput.str(FileCache::GetFile(aFilename));
  else
    mOutput.open(aFilename.ToCharArray());

  if(aMode == MODE_INPUT)
  {
    Parse();
  }
  else if(aMode == MODE_OUTPUT && mOutput.good())
  {
    mDictionary = new ParserNode();
    mDictionary->SetName("Start");
  }
  else
  {
    std::cout << "Input file: " << aFilename << " not found!" << std::endl;
    assert(!"Input file not found, check the logs");
  }
}
TextParser::~TextParser()
{
  if(mMode == MODE_OUTPUT && !mWrittenOut)
    Write();
  mOutput.close();
}

/**
 * @brief Parse text file
 */
void TextParser::Parse()
{
  ParserNode *mCurNode = nullptr;

  while(!mInput.eof())
  {
    std::string type;
    std::string name;
    mInput >> name;

    // If a closing bracket, the node is done
    if(name == "}")
    {
      if(mCurNode->GetParent())
        mCurNode = mCurNode->GetParent();
      continue;
    }

    ParserNode *node = new ParserNode();
    node->SetName(name);
    mInput >> type;

    // We found our value
    if(type == "=")
    {
      std::string value;
      mInput >> value;
      node->SetValue(Common::ParseString(&mInput, value));
      node->SetParent(mCurNode);
      mCurNode->Insert(node);
    }
    // Start a new child node
    else if(type == "{")
    {
      if(mCurNode)
        mCurNode->Insert(node);

      node->SetParent(mCurNode);
      mCurNode = node;
    }
    // Close the node
    else if(type == "}")
    {
      delete node;
      if(mCurNode->GetParent())
        mCurNode = mCurNode->GetParent();
    }
    else
    {
      delete node;
    }
  }

  mDictionary = mCurNode;
}

/**
 * @brief Write out to text file.
 */
void TextParser::Write()
{
  mWrittenOut = true;
  WriteRoot(mDictionary);
}

/**
 * @brief Write out root and all children below it.
 * @param aRoot Root to write
 */
void TextParser::WriteRoot(ParserNode *aRoot)
{
  if(aRoot->GetValue() == "")
  {
    // Sorting for cleanliness
    std::vector<ParserNode*> roots;
    std::copy(aRoot->GetChildren().begin(), aRoot->GetChildren().end(), std::back_inserter(roots));
    std::sort(roots.begin(), roots.end(), RootSortPredicate);
    std::vector<ParserNode*>::iterator begin = roots.begin();
    std::vector<ParserNode*>::iterator end = roots.end();
    mOutput << InsertIndents() << aRoot->GetName() << std::endl;
    mOutput << InsertIndents() << "{" << std::endl;
    ++mCurrentIndent;
    for(std::vector<ParserNode*>::iterator it = begin; it != end; ++it)
    {
      WriteRoot(*it);
    }
    --mCurrentIndent;
    mOutput << InsertIndents() << "}" << std::endl;
  }
  else
  {
    mOutput << InsertIndents() << aRoot->GetName() << " = " << aRoot->GetValue() << std::endl;
  }
}

/**
 * @brief Indentation helper.
 */
std::string TextParser::InsertIndents()
{
  std::string ret;
  for(int i = 0; i < mCurrentIndent; ++i)
  {
    ret.push_back('\t');
  }
  return ret;
}
