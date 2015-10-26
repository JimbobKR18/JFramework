#include "Transform.h"
#include "TextParser.h"
#include "LuaIncludes.h"

int const Transform::sUID = Common::StringHashFunction("Transform");

Transform::Transform() : Component(Transform::sUID), mPosition(0, 0, 0), mScale(1, 1, 1), mRotation(),
                         mXAlign(X_ALIGN_CENTER), mYAlign(Y_ALIGN_CENTER), mZAlign(Z_ALIGN_CENTER)
{
}

Transform::~Transform()
{
}

/**
 * @brief Get position of transform
 * @return position
 */
Vector3& Transform::GetPosition()
{
  return mPosition;
}

/**
 * @brief Set position of transform
 * @param aPos position
 */
void Transform::SetPosition(Vector3 const &aPos)
{
  mPosition = aPos;
}

/**
 * @brief Get scale of transform
 * @return scale
 */
Vector3& Transform::GetScale()
{
  return mScale;
}

/**
 * @brief Set scale of transform (scalar)
 * @param aScale scale
 */
void Transform::SetScale(Vector3 const &aScale)
{
  mScale = aScale;
}

/**
 * @brief Get size of transform (linear)
 * @return size
 */
Vector3& Transform::GetSize()
{
  return mSize;
}

/**
 * @brief Set size of transform
 * @param aSize size
 */
void Transform::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
}

/**
 * @brief Get rotation of transform
 * @return rotation
 */
Matrix33& Transform::GetRotation()
{
  return mRotation;
}

/**
 * @brief Set rotation of transform
 * @param aRotation transform
 */
void Transform::SetRotation(Matrix33 const &aRotation)
{
  mRotation = aRotation;
}

/**
 * @brief Get x alignment of transform
 * @return alignment
 */
X_ALIGNMENT Transform::GetXAlignment() const
{
  return mXAlign;
}

/**
 * @brief Set x alignment of transform
 * @param aAlign alignment
 */
void Transform::SetXAlignment(X_ALIGNMENT const &aAlign)
{
  mXAlign = aAlign;
}

/**
 * @brief Get y alignment of transform
 * @return alignment
 */
Y_ALIGNMENT Transform::GetYAlignment() const
{
  return mYAlign;
}

/**
 * @brief Set y alignment of transform
 * @param aAlign alignment
 */
void Transform::SetYAlignment(Y_ALIGNMENT const &aAlign)
{
  mYAlign = aAlign;
}

/**
 * @brief Get z alignment of transform
 * @return alignment
 */
Z_ALIGNMENT Transform::GetZAlignment() const
{
  return mZAlign;
}

/**
 * @brief Set z alignment of transform
 * @param aAlign alignment
 */
void Transform::SetZAlignment(Z_ALIGNMENT const &aAlign)
{
  mZAlign = aAlign;
}

/**
 * @brief Serialize to file (cannot handle rotations)
 * @param aParser File to serialize to
 */
void Transform::Serialize(Parser &aParser)
{
  // NOTE rotations are kinda dumb to do here...
  std::string objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  Root* object = aParser.Find(objectName);
  char const *values[9] = {"PositionX",
                            "PositionY",
                            "PositionZ",
                            "ScaleX",
                            "ScaleY",
                            "ScaleZ",
                            "SizeX",
                            "SizeY",
                            "SizeZ"};
  object->Place(objectName, "Transform", "");
  for(int i = 0; i < 9; ++i)
  {
    float value = 0;
    if(i < 3)
      value = mPosition[i];
    else if(i >= 3 && i < 6)
      value = mScale[i - 3];
    else
      value = mSize[i - 6];
    object->Place("Transform", values[i], Common::FloatToString(value));
  }

  if(mXAlign == X_ALIGN_LEFT)
    object->Place("Transform", "AlignX", "LEFT");
  else if(mXAlign == X_ALIGN_CENTER)
    object->Place("Transform", "AlignX", "CENTER");
  else if(mXAlign == X_ALIGN_RIGHT)
    object->Place("Transform", "AlignX", "RIGHT");

  if(mYAlign == Y_ALIGN_TOP)
    object->Place("Transform", "AlignY", "TOP");
  else if(mYAlign == Y_ALIGN_CENTER)
    object->Place("Transform", "AlignY", "CENTER");
  else if(mYAlign == Y_ALIGN_BOTTOM)
    object->Place("Transform", "AlignY", "BOTTOM");

  if(mZAlign == Z_ALIGN_FRONT)
    object->Place("Transform", "AlignZ", "FRONT");
  else if(mZAlign == Z_ALIGN_CENTER)
    object->Place("Transform", "AlignZ", "CENTER");
  else if(mZAlign == Z_ALIGN_BACK)
    object->Place("Transform", "AlignZ", "BACK");
}

/**
 * @brief Read from file
 * @param aParser File to read from
 */
void Transform::Deserialize(Parser &aParser)
{
  // Position, Scale, Size
  SetPosition(Vector3(aParser.Find("Transform", "PositionX")->GetValue().ToFloat(),
                      aParser.Find("Transform", "PositionY")->GetValue().ToFloat(),
                      aParser.Find("Transform", "PositionZ")->GetValue().ToFloat()));
  SetScale(Vector3(aParser.Find("Transform", "ScaleX")->GetValue().ToFloat(),
                   aParser.Find("Transform", "ScaleY")->GetValue().ToFloat(),
                   aParser.Find("Transform", "ScaleZ")->GetValue().ToFloat()));
  SetSize(Vector3(aParser.Find("Transform", "SizeX")->GetValue().ToFloat(),
                  aParser.Find("Transform", "SizeY")->GetValue().ToFloat(),
                  aParser.Find("Transform", "SizeZ")->GetValue().ToFloat()));

  // Alignment
  HashString xAlign = aParser.Find("Transform", "AlignX")->GetValue();
  HashString yAlign = aParser.Find("Transform", "AlignY")->GetValue();
  HashString zAlign = aParser.Find("Transform", "AlignZ")->GetValue();
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
  
  // Rotation
  HashString xRotation = aParser.Find("Transform", "RotationX")->GetValue();
  HashString yRotation = aParser.Find("Transform", "RotationY")->GetValue();
  HashString zRotation = aParser.Find("Transform", "RotationZ")->GetValue();
  if(xRotation != "")
  {
    mRotation = mRotation.Rotate(Vector3(1,0,0), xRotation.ToFloat());
  }
  if(yRotation != "")
  {
    mRotation = mRotation.Rotate(Vector3(0,1,0), yRotation.ToFloat());
  }
  if(zRotation != "")
  {
    mRotation = mRotation.Rotate(Vector3(0,0,1), zRotation.ToFloat());
  }
}

/**
 * @brief Make readable in LUA (should already be called for you)
 */
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
