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
  aParser.Place("Transform", "PositionX = ");
  aParser.Place("Transform", "PositionY = ");
  aParser.Place("Transform", "PositionZ = ");
  aParser.Place("Transform", "ScaleX = ");
  aParser.Place("Transform", "ScaleY = ");
  aParser.Place("Transform", "ScaleZ = ");
  aParser.Place("Transform", "SizeX = ");
  aParser.Place("Transform", "SizeX = ");
  aParser.Place("Transform", "SizeY = ");
  aParser.Place("Transform", "SizeZ = ");
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
