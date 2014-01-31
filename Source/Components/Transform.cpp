#include "Transform.h"
#include "TextParser.h"
#include "LuaIncludes.h"

Transform::Transform() : Component("Transform"), mPosition(0, 0, 0), mScale(1, 1, 1)
{
}

Transform::~Transform()
{
}

Vector3& Transform::GetPosition()
{
  return mPosition;
}
void Transform::SetPosition(Vector3 const &aPos)
{
  mPosition = aPos;
}

Vector3& Transform::GetScale()
{
  return mScale;
}
void Transform::SetScale(Vector3 const &aScale)
{
  mScale = aScale;
}

Vector3& Transform::GetSize()
{
  return mSize;
}
void Transform::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
}

void Transform::Serialize(Parser &aParser)
{
  char const *values[9] = {"PositionX",
                            "PositionY",
                            "PositionZ",
                            "ScaleX",
                            "ScaleY",
                            "ScaleZ",
                            "SizeX",
                            "SizeY",
                            "SizeZ"};
  aParser.Place("Transform", "");
  for(int i = 0; i < 9; ++i)
  {
    float value = 0;
    if(i < 3)
      value = mPosition[i];
    else if(i >= 3 && i < 7)
      value = mScale[i - 3];
    else
      value = mSize[i - 7];
    aParser.Place("Transform", values[i], Common::FloatToString(value));
  }
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

void Transform::SerializeLUA()
{
  SLB::Class<Transform>("Transform").constructor()
          .set("GetPosition", &Transform::GetPosition)
          .set("GetScale", &Transform::GetScale)
          .set("GetSize", &Transform::GetSize)
          .set("SetPosition", &Transform::SetPosition)
          .set("SetScale", &Transform::SetScale)
          .set("SetSize", &Transform::SetSize);
}
