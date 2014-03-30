#include "Transform.h"
#include "TextParser.h"
#include "LuaIncludes.h"

Transform::Transform() : Component("Transform"), mPosition(0, 0, 0), mScale(1, 1, 1),
                         mXAlign(X_ALIGN_CENTER), mYAlign(Y_ALIGN_CENTER), mZAlign(Z_ALIGN_CENTER)
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

X_ALIGNMENT Transform::GetXAlignment() const
{
  return mXAlign;
}
void Transform::SetXAlignment(X_ALIGNMENT const &aAlign)
{
  mXAlign = aAlign;
}

Y_ALIGNMENT Transform::GetYAlignment() const
{
  return mYAlign;
}
void Transform::SetYAlignment(Y_ALIGNMENT const &aAlign)
{
  mYAlign = aAlign;
}

Z_ALIGNMENT Transform::GetZAlignment() const
{
  return mZAlign;
}
void Transform::SetZAlignment(Z_ALIGNMENT const &aAlign)
{
  mZAlign = aAlign;
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
    else if(i >= 3 && i < 6)
      value = mScale[i - 3];
    else
      value = mSize[i - 6];
    aParser.Place("Transform", values[i], Common::IntToString(value));
  }

  if(mXAlign == X_ALIGN_LEFT)
    aParser.Place("Transform", "AlignX", "LEFT");
  else if(mXAlign == X_ALIGN_CENTER)
    aParser.Place("Transform", "AlignX", "CENTER");
  else if(mXAlign == X_ALIGN_RIGHT)
      aParser.Place("Transform", "AlignX", "RIGHT");

  if(mYAlign == Y_ALIGN_TOP)
    aParser.Place("Transform", "AlignY", "TOP");
  else if(mYAlign == Y_ALIGN_CENTER)
    aParser.Place("Transform", "AlignY", "CENTER");
  else if(mYAlign == Y_ALIGN_BOTTOM)
    aParser.Place("Transform", "AlignY", "BOTTOM");

  if(mZAlign == Z_ALIGN_FRONT)
    aParser.Place("Transform", "AlignZ", "FRONT");
  else if(mZAlign == Z_ALIGN_CENTER)
    aParser.Place("Transform", "AlignZ", "CENTER");
  else if(mZAlign == Z_ALIGN_BACK)
    aParser.Place("Transform", "AlignZ", "BACK");
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

  std::string xAlign = aParser.Find("Transform", "AlignX");
  std::string yAlign = aParser.Find("Transform", "AlignY");
  std::string zAlign = aParser.Find("Transform", "AlignZ");
  if(xAlign != "")
  {
    if(xAlign == "LEFT")
      mXAlign = X_ALIGN_LEFT;
    else if(xAlign == "RIGHT")
      mXAlign = X_ALIGN_RIGHT;
    else if(xAlign != "CENTER")
      assert(!"Invalid value passed into XAlign");
  }
  if(yAlign != "")
  {
    if(yAlign == "TOP")
      mYAlign = Y_ALIGN_TOP;
    else if(yAlign == "BOTTOM")
      mYAlign = Y_ALIGN_BOTTOM;
    else if(yAlign != "CENTER")
      assert(!"Invalid value passed into YAlign");
  }
  if(zAlign != "")
  {
    if(zAlign == "FRONT")
      mZAlign = Z_ALIGN_FRONT;
    else if(zAlign == "BACK")
      mZAlign = Z_ALIGN_BACK;
    else if(zAlign != "CENTER")
      assert(!"Invalid value passed into ZAlign");
  }
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
