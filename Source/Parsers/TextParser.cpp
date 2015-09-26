#include "TextParser.h"
#include "Common.h"

TextParser::TextParser(std::string const &aFilename, TextMode const &aMode) : Parser(aFilename),
                                                                              mWrittenOut(false),
                                                                              mMode(aMode),
                                                                              mCurrentIndent(0)
{
  if(aMode == MODE_INPUT)
    mInput.open(aFilename.c_str());
  else
    mOutput.open(aFilename.c_str());

  if(aMode == MODE_INPUT && mInput.good())
  {
    Parse();
  }
  else if(aMode == MODE_OUTPUT && mOutput.good())
  {
    mDictionary = new Root();
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
  mInput.close();
}

void TextParser::Parse()
{
  Root *mCurNode = nullptr;

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

    Root *node = new Root();
    node->SetName(name);
    mInput >> type;

    // We found our value
    if(type == "=")
    {
      std::string value;
      mInput >> value;
      node->SetValue(ParseLiteral(value));
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

void TextParser::Write()
{
  mWrittenOut = true;
  WriteRoot(mDictionary);
}

void TextParser::WriteRoot(Root *aRoot)
{
  if(aRoot->GetValue().ToString() == "")
  {
    rootIT end = aRoot->GetChildren().end();
    mOutput << InsertIndents() << aRoot->GetName() << std::endl;
    mOutput << InsertIndents() << "{" << std::endl;
    ++mCurrentIndent;
    for(rootIT it = aRoot->GetChildren().begin(); it != end; ++it)
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

std::string TextParser::InsertIndents()
{
  std::string ret;
  for(int i = 0; i < mCurrentIndent; ++i)
  {
    ret.push_back('\t');
  }
  return ret;
}

std::string TextParser::ParseLiteral(std::string const &aLiteral)
{
  std::string ret;
  if(aLiteral.find("Literal") == std::string::npos)
    ret = ParseNegative(aLiteral);
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

std::string TextParser::ParseNegative(std::string const &aValue)
{
  std::string ret;
  ret = aValue;
  return ret;
}
