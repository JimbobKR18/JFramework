#include "Transform.h"
#include "LuaIncludes.h"

HashType const Transform::sUID = Common::StringHashFunction("Transform");
HashString const TRANSFORM = "Transform";
HashString const POSITIONX = "PositionX";
HashString const POSITIONY = "PositionY";
HashString const POSITIONZ = "PositionZ";
HashString const SCALEX = "ScaleX";
HashString const SCALEY = "ScaleY";
HashString const SCALEZ = "ScaleZ";
HashString const SIZEX = "SizeX";
HashString const SIZEY = "SizeY";
HashString const SIZEZ = "SizeZ";
HashString const MAXPOSITIONX = "MaxPositionX";
HashString const MAXPOSITIONY = "MaxPositionY";
HashString const MAXPOSITIONZ = "MaxPositionZ";
HashString const MINPOSITIONX = "MinPositionX";
HashString const MINPOSITIONY = "MinPositionY";
HashString const MINPOSITIONZ = "MinPositionZ";
HashString const ALIGNX = "AlignX";
HashString const ALIGNY = "AlignY";
HashString const ALIGNZ = "AlignZ";
HashString const ROTATIONX = "RotationX";
HashString const ROTATIONY = "RotationY";
HashString const ROTATIONZ = "RotationZ";
HashString const LOCKEDAXES = "LockedAxes";
HashString const XAXIS = "X_AXIS";
HashString const YAXIS = "Y_AXIS";
HashString const ZAXIS = "Z_AXIS";
HashString const XYAXIS = "XY_AXIS";
HashString const YZAXIS = "YZ_AXIS";
HashString const XZAXIS = "XZ_AXIS";
HashString const ALLAXES = "ALL_AXES";
HashString const NOAXIS = "NO_AXIS";
HashString const INHERITINFO = "InheritInfo";
HashString const INHERITNONE = "INHERIT_NONE";
HashString const INHERITPOSITION = "INHERIT_POSITION";
HashString const INHERITROTATION = "INHERIT_ROTATION";
HashString const INHERITSCALE = "INHERIT_SCALE";
HashString const INHERITPOSITIONROTATION = "INHERIT_POSITION_ROTATION";
HashString const INHERITROTATIONSCALE = "INHERIT_ROTATION_SCALE";
HashString const INHERITPOSITIONSCALE = "INHERIT_POSITION_SCALE";
HashString const INHERITALL = "INHERIT_ALL";
HashString const LEFT = "LEFT";
HashString const RIGHT = "RIGHT";
HashString const CENTER = "CENTER";
HashString const TOP = "TOP";
HashString const BOTTOM = "BOTTOM";
HashString const FRONT = "FRONT";
HashString const BACK = "BACK";

Transform::Transform() : Component(Transform::sUID), mPosition(), mScale(1, 1, 1), mSize(), mRotation(),
                         mXAlign(X_ALIGN_CENTER), mYAlign(Y_ALIGN_CENTER), mZAlign(Z_ALIGN_CENTER), mLockedAxes(NO_AXIS),
                         mInheritInfo(INHERIT_ALL), mHierarchicalPosition(), mHierarchicalScale(1, 1, 1), mHierarchicalRotation(),
                         mMaxBoundary(FLT_MAX, FLT_MAX, FLT_MAX), mMinBoundary(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{
}

Transform::Transform(Transform const &aTransform) : Component(Transform::sUID),
                                                    mPosition(aTransform.mPosition), mScale(aTransform.mScale),
                                                    mSize(aTransform.mSize), mRotation(aTransform.mRotation),
                                                    mXAlign(aTransform.mXAlign), mYAlign(aTransform.mYAlign),
                                                    mZAlign(aTransform.mZAlign), mLockedAxes(aTransform.mLockedAxes),
                                                    mInheritInfo(aTransform.mInheritInfo), mHierarchicalPosition(aTransform.mHierarchicalPosition),
                                                    mHierarchicalScale(aTransform.mHierarchicalScale),
                                                    mHierarchicalRotation(aTransform.mHierarchicalRotation),
                                                    mMaxBoundary(aTransform.mMaxBoundary), mMinBoundary(aTransform.mMinBoundary)
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
 * @brief Get max boundary.
 * @return Max boundary.
 */
Vector3 Transform::GetMaxBoundary() const
{
  return mMaxBoundary;
}

/**
 * @brief Get min boundary.
 * @return Min boundary.
 */
Vector3 Transform::GetMinBoundary() const
{
  return mMinBoundary;
}

Matrix33 Transform::GetFinalTransform() const
{
  return mRotation * Matrix33(mScale);
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
  EnforceBoundaries();
  CalculateHierarchy();
}

/**
 * @brief Set scale of transform (scalar)
 * @param aScale scale
 */
void Transform::SetScale(Vector3 const &aScale)
{
  mScale = aScale;
  EnforceBoundaries();
  CalculateHierarchy();
}

/**
 * @brief Set size of transform
 * @param aSize size
 */
void Transform::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
  EnforceBoundaries();
  CalculateHierarchy();
}

/**
 * @brief Set rotation of transform
 * @param aRotation transform
 */
void Transform::SetRotation(Matrix33 const &aRotation)
{
  mRotation = aRotation;
  EnforceBoundaries();
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
 * @brief Set max boundary.
 * @param aMaxBoundary Max boundary.
 */
void Transform::SetMaxBoundary(Vector3 const &aMaxBoundary)
{
  mMaxBoundary = aMaxBoundary;
}

/**
 * @brief Set min boundary.
 * @param aMinBoundary Min boundary.
 */
void Transform::SetMinBoundary(Vector3 const &aMinBoundary)
{
  mMinBoundary = aMinBoundary;
}

/**
 * @brief Update loop
 */
void Transform::Update()
{
  EnforceBoundaries();
  CalculateHierarchy();
}

/**
 * @brief Serialize to file (cannot handle rotations)
 * @param aNode ParserNode to serialize to.
 */
void Transform::Serialize(ParserNode *aNode)
{
  HashString const values[9] = {POSITIONX,
                            POSITIONY,
                            POSITIONZ,
                            SCALEX,
                            SCALEY,
                            SCALEZ,
                            SIZEX,
                            SIZEY,
                            SIZEZ};
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
    transform->Place(ALIGNX, LEFT);
  else if(mXAlign == X_ALIGN_CENTER)
    transform->Place(ALIGNX, CENTER);
  else if(mXAlign == X_ALIGN_RIGHT)
    transform->Place(ALIGNX, RIGHT);

  if(mYAlign == Y_ALIGN_TOP)
    transform->Place(ALIGNY, TOP);
  else if(mYAlign == Y_ALIGN_CENTER)
    transform->Place(ALIGNY, CENTER);
  else if(mYAlign == Y_ALIGN_BOTTOM)
    transform->Place(ALIGNY, BOTTOM);

  if(mZAlign == Z_ALIGN_FRONT)
    transform->Place(ALIGNZ, FRONT);
  else if(mZAlign == Z_ALIGN_CENTER)
    transform->Place(ALIGNZ, CENTER);
  else if(mZAlign == Z_ALIGN_BACK)
    transform->Place(ALIGNZ, BACK);
    
  if(mLockedAxes == NO_AXIS)
    transform->Place(LOCKEDAXES, NOAXIS);
  else if(mLockedAxes == X_AXIS)
    transform->Place(LOCKEDAXES, XAXIS);
  else if(mLockedAxes == Y_AXIS)
    transform->Place(LOCKEDAXES, YAXIS);
  else if(mLockedAxes == Z_AXIS)
    transform->Place(LOCKEDAXES, ZAXIS);
  else if(mLockedAxes == XY_AXIS)
    transform->Place(LOCKEDAXES, XYAXIS);
  else if(mLockedAxes == YZ_AXIS)
    transform->Place(LOCKEDAXES, YZAXIS);
  else if(mLockedAxes == XZ_AXIS)
    transform->Place(LOCKEDAXES, XZAXIS);
  else if(mLockedAxes == ALL_AXES)
    transform->Place(LOCKEDAXES, ALLAXES);
    
  if(mInheritInfo == INHERIT_NONE)
    transform->Place(INHERITINFO, INHERITNONE);
  else if(mInheritInfo == INHERIT_POSITION)
    transform->Place(INHERITINFO, INHERITPOSITION);
  else if(mInheritInfo == INHERIT_ROTATION)
    transform->Place(INHERITINFO, INHERITROTATION);
  else if(mInheritInfo == INHERIT_SCALE)
    transform->Place(INHERITINFO, INHERITSCALE);
  else if(mInheritInfo == INHERIT_POSITION_ROTATION)
    transform->Place(INHERITINFO, INHERITPOSITIONROTATION);
  else if(mInheritInfo == INHERIT_ROTATION_SCALE)
    transform->Place(INHERITINFO, INHERITROTATIONSCALE);
  else if(mInheritInfo == INHERIT_POSITION_SCALE)
    transform->Place(INHERITINFO, INHERITPOSITIONSCALE);
  else if(mInheritInfo == INHERIT_ALL)
    transform->Place(INHERITINFO, INHERITALL);
    
  // Rotations are a little TOO complicated, so set them to 0
  transform->Place(ROTATIONX, Common::IntToString(0));
  transform->Place(ROTATIONY, Common::IntToString(0));
  transform->Place(ROTATIONZ, Common::IntToString(0));
  
  // Boundaries
  transform->Place(MAXPOSITIONX, Common::FloatToString(mMaxBoundary.x));
  transform->Place(MAXPOSITIONY, Common::FloatToString(mMaxBoundary.y));
  transform->Place(MAXPOSITIONZ, Common::FloatToString(mMaxBoundary.z));
  transform->Place(MINPOSITIONX, Common::FloatToString(mMinBoundary.x));
  transform->Place(MINPOSITIONY, Common::FloatToString(mMinBoundary.y));
  transform->Place(MINPOSITIONZ, Common::FloatToString(mMinBoundary.z));
}

/**
 * @brief Read from file
 * @param aNode ParserNode to read from.
 */
void Transform::Deserialize(ParserNode *aNode)
{
  // Position, Scale, Size
  Vector3 position = mPosition;
  if(aNode->Find(POSITIONX))
    position.x = aNode->Find(POSITIONX)->GetValue().ToFloat();
  if(aNode->Find(POSITIONY))
    position.y = aNode->Find(POSITIONY)->GetValue().ToFloat();
  if(aNode->Find(POSITIONZ))
    position.z = aNode->Find(POSITIONZ)->GetValue().ToFloat();
  SetPosition(position);
  
  // WARNING: Setting scale to 0 causes a bug where the renderer doesn't draw them.
  if(aNode->Find(SCALEX))
    mScale.x = aNode->Find(SCALEX)->GetValue().ToFloat();
  if(aNode->Find(SCALEY))
    mScale.y = aNode->Find(SCALEY)->GetValue().ToFloat();
  if(aNode->Find(SCALEZ))
    mScale.z = aNode->Find(SCALEZ)->GetValue().ToFloat();
    
  if(aNode->Find(SIZEX))
    mSize.x = aNode->Find(SIZEX)->GetValue().ToFloat();
  if(aNode->Find(SIZEY))
    mSize.y = aNode->Find(SIZEY)->GetValue().ToFloat();
  if(aNode->Find(SIZEZ))
    mSize.z = aNode->Find(SIZEZ)->GetValue().ToFloat();
  
  // Boundaries
  if(aNode->Find(MAXPOSITIONX))
    mMaxBoundary.x = aNode->Find(MAXPOSITIONX)->GetValue().ToFloat();
  if(aNode->Find(MAXPOSITIONY))
    mMaxBoundary.y = aNode->Find(MAXPOSITIONY)->GetValue().ToFloat();
  if(aNode->Find(MAXPOSITIONZ))
    mMaxBoundary.z = aNode->Find(MAXPOSITIONZ)->GetValue().ToFloat();
    
  if(aNode->Find(MINPOSITIONX))
    mMinBoundary.x = aNode->Find(MINPOSITIONX)->GetValue().ToFloat();
  if(aNode->Find(MINPOSITIONY))
    mMinBoundary.y = aNode->Find(MINPOSITIONY)->GetValue().ToFloat();
  if(aNode->Find(MINPOSITIONZ))
    mMinBoundary.z = aNode->Find(MINPOSITIONZ)->GetValue().ToFloat();

  // Alignment
  HashString xAlign = aNode->Find(ALIGNX) ? aNode->Find(ALIGNX)->GetValue() : "";
  HashString yAlign = aNode->Find(ALIGNY) ? aNode->Find(ALIGNY)->GetValue() : "";
  HashString zAlign = aNode->Find(ALIGNZ) ? aNode->Find(ALIGNZ)->GetValue() : "";
  if(xAlign != "")
  {
    if(xAlign == LEFT)
      mXAlign = X_ALIGN_LEFT;
    else if(xAlign == RIGHT)
      mXAlign = X_ALIGN_RIGHT;
    else if(xAlign != CENTER)
      assert(!"Invalid value passed into XAlign");
  }
  if(yAlign != "")
  {
    if(yAlign == TOP)
      mYAlign = Y_ALIGN_TOP;
    else if(yAlign == BOTTOM)
      mYAlign = Y_ALIGN_BOTTOM;
    else if(yAlign != CENTER)
      assert(!"Invalid value passed into YAlign");
  }
  if(zAlign != "")
  {
    if(zAlign == FRONT)
      mZAlign = Z_ALIGN_FRONT;
    else if(zAlign == BACK)
      mZAlign = Z_ALIGN_BACK;
    else if(zAlign != CENTER)
      assert(!"Invalid value passed into ZAlign");
  }
  
  // Rotation
  if(aNode->Find(ROTATIONX))
    mRotation = mRotation.Rotate(Vector3(1,0,0), aNode->Find(ROTATIONX)->GetValue().ToFloat());
  if(aNode->Find(ROTATIONY))
    mRotation = mRotation.Rotate(Vector3(0,1,0), aNode->Find(ROTATIONY)->GetValue().ToFloat());
  if(aNode->Find(ROTATIONZ))
    mRotation = mRotation.Rotate(Vector3(0,0,1), aNode->Find(ROTATIONZ)->GetValue().ToFloat());
  
  // Axis lock
  if(aNode->Find(LOCKEDAXES))
  {
    HashString axisLock = aNode->Find(LOCKEDAXES)->GetValue();
    if(axisLock == XAXIS)
      mLockedAxes = X_AXIS;
    else if(axisLock == YAXIS)
      mLockedAxes = Y_AXIS;
    else if(axisLock == ZAXIS)
      mLockedAxes = Z_AXIS;
    else if(axisLock == XYAXIS)
      mLockedAxes = XY_AXIS;
    else if(axisLock == YZAXIS)
      mLockedAxes = YZ_AXIS;
    else if(axisLock == XZAXIS)
      mLockedAxes = XZ_AXIS;
    else if(axisLock == ALLAXES)
      mLockedAxes = ALL_AXES;
    else if(axisLock == NOAXIS)
      mLockedAxes = NO_AXIS;
    else
      assert(!"Invalid axis lock value passed in.");
  }
  
  // Parent inherit info
  if(aNode->Find(INHERITINFO))
  {
    HashString inheritInfo = aNode->Find(INHERITINFO)->GetValue();
    if(inheritInfo == INHERITNONE)
      mInheritInfo = INHERIT_NONE;
    else if(inheritInfo == INHERITPOSITION)
      mInheritInfo = INHERIT_POSITION;
    else if(inheritInfo == INHERITROTATION)
      mInheritInfo = INHERIT_ROTATION;
    else if(inheritInfo == INHERITSCALE)
      mInheritInfo = INHERIT_SCALE;
    else if(inheritInfo == INHERITPOSITIONROTATION)
      mInheritInfo = INHERIT_POSITION_ROTATION;
    else if(inheritInfo == INHERITROTATIONSCALE)
      mInheritInfo = INHERIT_ROTATION_SCALE;
    else if(inheritInfo == INHERITPOSITIONSCALE)
      mInheritInfo = INHERIT_POSITION_SCALE;
    else if(inheritInfo == INHERITALL)
      mInheritInfo = INHERIT_ALL;
    else
      assert(!"Invalid inheritance value passed in.");
  }
  
  CalculateHierarchy();
}

/**
 * @brief Copy transform
 * @param aNewOwner The new owner
 * @return Copied transform
 */
Component* Transform::Clone(GameObject *aNewOwner) const
{
  return new Transform(*this);
}

/**
 * @brief Make readable in LUA (should already be called for you)
 */
void Transform::SerializeLUA()
{
  SLB::Class<Transform>("Transform").constructor()
    .inherits<Component>()
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
  
  // Inheritance.
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
  
  // Cascade calculations down the chain.
  GameObject::GameObjectContainer children = owner->GetChildren();
  GameObject::GameObjectIT end = children.end();
  for(GameObject::GameObjectIT it = children.begin(); it != end; ++it)
  {
    GameObject *child = it->second;
    child->GET<Transform>()->CalculateHierarchy();
  }
}

/**
 * @brief Make sure object is within bounds.
 */
void Transform::EnforceBoundaries()
{
  // This means that the camera, when zoomed in, cannot hit the sides of levels.
  // If you want to do that, you need to divide.
  Vector3 scaledSize = mSize.Multiply(mScale);
  if(mPosition.x - scaledSize.x < mMinBoundary.x)
    mPosition.x = mMinBoundary.x + scaledSize.x;
  else if(mPosition.x + scaledSize.x > mMaxBoundary.x)
    mPosition.x = mMaxBoundary.x - scaledSize.x;

  if(mPosition.y - scaledSize.y < mMinBoundary.y)
    mPosition.y = mMinBoundary.y + scaledSize.y;
  else if(mPosition.y + scaledSize.y > mMaxBoundary.y)
    mPosition.y = mMaxBoundary.y - scaledSize.y;
}
