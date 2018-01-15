#include "Transform.h"
#include "TextParser.h"
#include "LuaIncludes.h"

int const Transform::sUID = Common::StringHashFunction("Transform");

Transform::Transform() : Component(Transform::sUID), mPosition(), mScale(1, 1, 1), mSize(), mRotation(),
                         mXAlign(X_ALIGN_CENTER), mYAlign(Y_ALIGN_CENTER), mZAlign(Z_ALIGN_CENTER), mLockedAxes(NO_AXIS),
                         mInheritInfo(INHERIT_ALL), mHierarchicalPosition(), mHierarchicalScale(1, 1, 1), mHierarchicalRotation()
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
 * @brief Get scale of transform
 * @return scale
 */
Vector3& Transform::GetScale()
{
  return mScale;
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
 * @brief Get rotation of transform
 * @return rotation
 */
Matrix33& Transform::GetRotation()
{
  return mRotation;
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
 * @brief Get y alignment of transform
 * @return alignment
 */
Y_ALIGNMENT Transform::GetYAlignment() const
{
  return mYAlign;
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
 * @brief Get currently locked axes.
 * @return Currently locked axes.
 */
AxisLock Transform::GetLockedAxes() const
{
  return mLockedAxes;
}

/**
 * @brief Get parent inheritance info.
 * @return Inheritance info.
 */
ParentInherit Transform::GetParentInheritanceInfo() const
{
  return mInheritInfo;
}

/**
 * @brief Get position, taking into account parents.
 * @return Hierarchical position.
 */
Vector3 const& Transform::GetHierarchicalPosition() const
{
  return mHierarchicalPosition;
}

/**
 * @brief Get scale, taking into account parents.
 * @return Hierarchical scale.
 */
Vector3 const& Transform::GetHierarchicalScale() const
{
  return mHierarchicalScale;
}

/**
 * @brief Get rotation, taking into account parents.
 * @return Hierarchical rotation.
 */
Matrix33 const& Transform::GetHierarchicalRotation() const
{
  return mHierarchicalRotation;
}

/**
 * @brief Set position of transform
 * @param aPos position
 */
void Transform::SetPosition(Vector3 const &aPos)
{
  switch(mLockedAxes)
  {
    case NO_AXIS:
      mPosition = aPos;
      break;
    case X_AXIS:
      mPosition.y = aPos.y;
      mPosition.z = aPos.z;
      break;
    case Y_AXIS:
      mPosition.x = aPos.x;
      mPosition.z = aPos.z;
      break;
    case Z_AXIS:
      mPosition.x = aPos.x;
      mPosition.y = aPos.y;
      break;
    case XY_AXIS:
      mPosition.z = aPos.z;
      break;
    case YZ_AXIS:
      mPosition.x = aPos.x;
      break;
    case XZ_AXIS:
      mPosition.y = aPos.y;
      break;
    case ALL_AXES:
      break;
    default:
      assert(!"How did you even get here?");
      break;
  }
  CalculateHierarchy();
}

/**
 * @brief Set scale of transform (scalar)
 * @param aScale scale
 */
void Transform::SetScale(Vector3 const &aScale)
{
  mScale = aScale;
  CalculateHierarchy();
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
 * @brief Set rotation of transform
 * @param aRotation transform
 */
void Transform::SetRotation(Matrix33 const &aRotation)
{
  mRotation = aRotation;
  CalculateHierarchy();
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
 * @brief Set y alignment of transform
 * @param aAlign alignment
 */
void Transform::SetYAlignment(Y_ALIGNMENT const &aAlign)
{
  mYAlign = aAlign;
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
 * @brief Set currently locked axes.
 * @param aLockedAxes Locked axes.
 */
void Transform::SetLockedAxis(AxisLock const &aLockedAxes)
{
  mLockedAxes = aLockedAxes;
}

/**
 * @brief Set what will be inherited by object's parent.
 * @param aInheritInfo Inheritance info.
 */
void Transform::SetParentInheritanceInfo(ParentInherit const &aInheritInfo)
{
  mInheritInfo = aInheritInfo;
}

/**
 * @brief Update loop
 */
void Transform::Update()
{
  CalculateHierarchy();
}

/**
 * @brief Serialize to file (cannot handle rotations)
 * @param aNode ParserNode to serialize to.
 */
void Transform::Serialize(ParserNode *aNode)
{
  HashString const TRANSFORM = "Transform";
  char const *values[9] = {"PositionX",
                            "PositionY",
                            "PositionZ",
                            "ScaleX",
                            "ScaleY",
                            "ScaleZ",
                            "SizeX",
                            "SizeY",
                            "SizeZ"};
  aNode->Place(TRANSFORM, "");
  ParserNode* transform = aNode->Find(TRANSFORM);
  for(int i = 0; i < 9; ++i)
  {
    float value = 0;
    if(i < 3)
      value = mPosition[i];
    else if(i >= 3 && i < 6)
      value = mScale[i - 3];
    else
      value = mSize[i - 6];
    transform->Place(values[i], Common::FloatToString(value));
  }

  if(mXAlign == X_ALIGN_LEFT)
    transform->Place("AlignX", "LEFT");
  else if(mXAlign == X_ALIGN_CENTER)
    transform->Place("AlignX", "CENTER");
  else if(mXAlign == X_ALIGN_RIGHT)
    transform->Place("AlignX", "RIGHT");

  if(mYAlign == Y_ALIGN_TOP)
    transform->Place("AlignY", "TOP");
  else if(mYAlign == Y_ALIGN_CENTER)
    transform->Place("AlignY", "CENTER");
  else if(mYAlign == Y_ALIGN_BOTTOM)
    transform->Place("AlignY", "BOTTOM");

  if(mZAlign == Z_ALIGN_FRONT)
    transform->Place("AlignZ", "FRONT");
  else if(mZAlign == Z_ALIGN_CENTER)
    transform->Place("AlignZ", "CENTER");
  else if(mZAlign == Z_ALIGN_BACK)
    transform->Place("AlignZ", "BACK");
    
  if(mLockedAxes == NO_AXIS)
    transform->Place("LockedAxes", "NO_AXIS");
  else if(mLockedAxes == X_AXIS)
    transform->Place("LockedAxes", "X_AXIS");
  else if(mLockedAxes == Y_AXIS)
    transform->Place("LockedAxes", "Y_AXIS");
  else if(mLockedAxes == Z_AXIS)
    transform->Place("LockedAxes", "Z_AXIS");
  else if(mLockedAxes == XY_AXIS)
    transform->Place("LockedAxes", "XY_AXIS");
  else if(mLockedAxes == YZ_AXIS)
    transform->Place("LockedAxes", "YZ_AXIS");
  else if(mLockedAxes == XZ_AXIS)
    transform->Place("LockedAxes", "XZ_AXIS");
  else if(mLockedAxes == ALL_AXES)
    transform->Place("LockedAxes", "ALL_AXES");
    
  if(mInheritInfo == INHERIT_NONE)
    transform->Place("InheritInfo", "INHERIT_NONE");
  else if(mInheritInfo == INHERIT_POSITION)
    transform->Place("InheritInfo", "INHERIT_POSITION");
  else if(mInheritInfo == INHERIT_ROTATION)
    transform->Place("InheritInfo", "INHERIT_ROTATION");
  else if(mInheritInfo == INHERIT_SCALE)
    transform->Place("InheritInfo", "INHERIT_SCALE");
  else if(mInheritInfo == INHERIT_POSITION_ROTATION)
    transform->Place("InheritInfo", "INHERIT_POSITION_ROTATION");
  else if(mInheritInfo == INHERIT_ROTATION_SCALE)
    transform->Place("InheritInfo", "INHERIT_ROTATION_SCALE");
  else if(mInheritInfo == INHERIT_POSITION_SCALE)
    transform->Place("InheritInfo", "INHERIT_POSITION_SCALE");
  else if(mInheritInfo == INHERIT_ALL)
    transform->Place("InheritInfo", "INHERIT_ALL");
    
  // Rotations are a little TOO complicated, so set them to 0
  transform->Place("RotationX", Common::IntToString(0));
  transform->Place("RotationY", Common::IntToString(0));
  transform->Place("RotationZ", Common::IntToString(0));
}

/**
 * @brief Read from file
 * @param aNode ParserNode to read from.
 */
void Transform::Deserialize(ParserNode *aNode)
{
  // Position, Scale, Size
  if(aNode->Find("PositionX"))
    SetPosition(Vector3(aNode->Find("PositionX")->GetValue().ToFloat(),
                        aNode->Find("PositionY")->GetValue().ToFloat(),
                        aNode->Find("PositionZ")->GetValue().ToFloat()));
  if(aNode->Find("ScaleX"))
    SetScale(Vector3(aNode->Find("ScaleX")->GetValue().ToFloat(),
                     aNode->Find("ScaleY")->GetValue().ToFloat(),
                     aNode->Find("ScaleZ")->GetValue().ToFloat()));
  if(aNode->Find("SizeX"))
    SetSize(Vector3(aNode->Find("SizeX")->GetValue().ToFloat(),
                    aNode->Find("SizeY")->GetValue().ToFloat(),
                    aNode->Find("SizeZ")->GetValue().ToFloat()));

  // Alignment
  HashString xAlign = aNode->Find("AlignX") ? aNode->Find("AlignX")->GetValue() : "";
  HashString yAlign = aNode->Find("AlignY") ? aNode->Find("AlignY")->GetValue() : "";
  HashString zAlign = aNode->Find("AlignZ") ? aNode->Find("AlignZ")->GetValue() : "";
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
  if(aNode->Find("RotationX"))
    mRotation = mRotation.Rotate(Vector3(1,0,0), aNode->Find("RotationX")->GetValue().ToFloat());
  if(aNode->Find("RotationY"))
    mRotation = mRotation.Rotate(Vector3(0,1,0), aNode->Find("RotationY")->GetValue().ToFloat());
  if(aNode->Find("RotationZ"))
    mRotation = mRotation.Rotate(Vector3(0,0,1), aNode->Find("RotationZ")->GetValue().ToFloat());
  
  // Axis lock
  if(aNode->Find("LockedAxes"))
  {
    HashString axisLock = aNode->Find("LockedAxes")->GetValue();
    if(axisLock == "X_AXIS")
      mLockedAxes = X_AXIS;
    else if(axisLock == "Y_AXIS")
      mLockedAxes = Y_AXIS;
    else if(axisLock == "Z_AXIS")
      mLockedAxes = Z_AXIS;
    else if(axisLock == "XY_AXIS")
      mLockedAxes = XY_AXIS;
    else if(axisLock == "YZ_AXIS")
      mLockedAxes = YZ_AXIS;
    else if(axisLock == "XZ_AXIS")
      mLockedAxes = XZ_AXIS;
    else if(axisLock == "ALL_AXES")
      mLockedAxes = ALL_AXES;
    else if(axisLock == "NO_AXIS")
      mLockedAxes = NO_AXIS;
    else
      assert(!"Invalid axis lock value passed in.");
  }
  
  // Parent inherit info
  if(aNode->Find("InheritInfo"))
  {
    HashString inheritInfo = aNode->Find("InheritInfo")->GetValue();
    if(inheritInfo == "INHERIT_NONE")
      mInheritInfo = INHERIT_NONE;
    else if(inheritInfo == "INHERIT_POSITION")
      mInheritInfo = INHERIT_POSITION;
    else if(inheritInfo == "INHERIT_ROTATION")
      mInheritInfo = INHERIT_ROTATION;
    else if(inheritInfo == "INHERIT_SCALE")
      mInheritInfo = INHERIT_SCALE;
    else if(inheritInfo == "INHERIT_POSITION_ROTATION")
      mInheritInfo = INHERIT_POSITION_ROTATION;
    else if(inheritInfo == "INHERIT_ROTATION_SCALE")
      mInheritInfo = INHERIT_ROTATION_SCALE;
    else if(inheritInfo == "INHERIT_POSITION_SCALE")
      mInheritInfo = INHERIT_POSITION_SCALE;
    else if(inheritInfo == "INHERIT_ALL")
      mInheritInfo = INHERIT_ALL;
    else
      assert(!"Invalid inheritance value passed in.");
  }
}

/**
 * @brief Make readable in LUA (should already be called for you)
 */
void Transform::SerializeLUA()
{
  SLB::Class<Transform>("Transform").constructor()
          .set("GetPosition", &Transform::GetPosition)
          .set("GetRotation", &Transform::GetRotation)
          .set("GetScale", &Transform::GetScale)
          .set("GetSize", &Transform::GetSize)
          .set("SetPosition", &Transform::SetPosition)
          .set("SetRotation", &Transform::SetRotation)
          .set("SetScale", &Transform::SetScale)
          .set("SetSize", &Transform::SetSize);
}

/**
 * @brief Calculate all hierarchical data.
 */
void Transform::CalculateHierarchy()
{
  mHierarchicalPosition = mPosition;
  mHierarchicalScale = mScale;
  mHierarchicalRotation = mRotation;
  
  GameObject *owner = GetOwner();
  if(mInheritInfo != INHERIT_NONE && owner && owner->GetParent())
  {
    Transform *parentTransform = GetOwner()->GetParent()->GET<Transform>();
    if((mInheritInfo & INHERIT_POSITION) != 0)
    {
      if((mInheritInfo & INHERIT_ROTATION) != 0)
        mHierarchicalPosition = parentTransform->GetHierarchicalPosition() + (parentTransform->GetHierarchicalRotation() * mPosition);
      else
        mHierarchicalPosition = parentTransform->GetHierarchicalPosition() + mPosition;
    }
    if((mInheritInfo & INHERIT_SCALE) != 0)
      mHierarchicalScale = parentTransform->GetHierarchicalScale().Multiply(mScale);
    if((mInheritInfo & INHERIT_ROTATION) != 0)
      mHierarchicalRotation = parentTransform->GetHierarchicalRotation() * mRotation;
  }
}
