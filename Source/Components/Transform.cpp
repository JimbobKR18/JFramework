#include "Transform.h"
#include "TextParser.h"

Transform::Transform() : Component("Transform"), mPosition(0, 0, 0), mScale(1, 1, 1)
{
}

Transform::~Transform()
{
}

Vector3 Transform::GetPosition() const
{
  return mPosition;
}
void Transform::SetPosition(Vector3 const &aPos)
{
  mPosition = aPos;
}

Vector3 Transform::GetScale() const
{
  return mScale;
}
void Transform::SetScale(Vector3 const &aScale)
{
  mScale = aScale;
}

Vector3 Transform::GetSize() const
{
  return mSize;
}
void Transform::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
}

void Transform::Serialize(Parser &aParser)
{
  
}

void Transform::Deserialize(Parser &aParser)
{
  SetPosition(Vector3(Common::StringToInt(aParser.Find("Transform", "PositionX")),
                      Common::StringToInt(aParser.Find("Transform", "PositionY")),
                      Common::StringToInt(aParser.Find("Transform", "PositionZ"))));
  SetScale(Vector3(Common::StringToInt(aParser.Find("Transform", "ScaleX")),
                   Common::StringToInt(aParser.Find("Transform", "ScaleY")),
                   Common::StringToInt(aParser.Find("Transform", "ScaleZ"))));
  SetSize(Vector3(Common::StringToInt(aParser.Find("Transform", "SizeX")),
                  Common::StringToInt(aParser.Find("Transform", "SizeY")),
                  Common::StringToInt(aParser.Find("Transform", "SizeZ"))));
}
