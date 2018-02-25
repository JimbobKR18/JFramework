#include "Camera.h"
#include "GLFramebuffer.h"
#include "SystemProperties.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"

#if !defined(IOS) && !defined(ANDROID)
  #define SHADER_COMPATIBLE
#endif

int const Camera::sUID = Common::StringHashFunction("Camera");

Camera::Camera() : Component(Camera::sUID)
{
  assert(!"Cannot make Camera without GraphicsManager.");
}

Camera::Camera(GraphicsManager *aManager) : Component(Camera::sUID), 
  mSize(SystemProperties::GetRenderWidth(), SystemProperties::GetRenderHeight(), 0),
  mOffset(), mPrimary(false), mManager(aManager)
{
  #ifdef SHADER_COMPATIBLE
  mFramebuffer = new GLFramebuffer(mSize.x, mSize.y);
  #endif
  
  mFramebuffer->Generate(mManager);
}

Camera::~Camera()
{
  delete mFramebuffer;
  mManager->RemoveCamera(this);
}

// Methods
/**
 * @brief Get view size
 * @return View size
 */
Vector3 Camera::GetSize() const
{
  return mSize;
}

/**
 * @brief Set view size
 * @param aSize View size
 */
void Camera::SetSize(Vector3 const &aSize)
{
  mSize = aSize;
}

/**
 * @brief Get offset
 * @return Offset
 */
Vector3& Camera::GetOffset()
{
  return mOffset;
}

/**
 * @brief Set offset
 * @param aOffset Offset
 */
void Camera::SetOffset(Vector3 const &aOffset)
{
  mOffset = aOffset;
}

/**
 * @brief Get primary camera status
 * @return True if camera is primary (active)
 */
bool Camera::GetPrimary() const
{
  return mPrimary;
}

/**
 * @brief Set camera primary status
 * @param aPrimary Set to true if primary (active)
 */
void Camera::SetPrimary(bool aPrimary)
{
  mPrimary = aPrimary;
}

/**
 * @brief Get frame buffer
 * @return Frame buffer
 */
Framebuffer* Camera::GetFramebuffer()
{
  return mFramebuffer;
}

// Virtuals derived from Component
/**
 * @brief Does nothing
 */
void Camera::Update()
{  
}

/**
 * @brief Does nothing
 * @param aMessage
 */
void Camera::SendMessage(Message const& aMessage)
{
}

/**
 * @brief Does nothing
 * @param aMessage
 */
void Camera::ReceiveMessage(Message const& aMessage)
{
}

/**
 * @brief Serialize to file
 * @param aNode Node to write to
 */
void Camera::Serialize(ParserNode *aNode)
{
  HashString const CAMERA = "Camera";
  aNode->Place(CAMERA, "");
  ParserNode* object = aNode->Find(CAMERA);
  
  object->Place("Width", Common::IntToString(mSize.x));
  object->Place("Height", Common::IntToString(mSize.y));
  
  if(mManager->GetPrimaryCamera() == this)
    object->Place("Primary", Common::BoolToString(true));
}

/**
 * @brief Deserialize from file
 * @param aNode Node to read from
 */
void Camera::Deserialize(ParserNode *aNode)
{
  if(aNode->Find("Width"))
    mSize.x = aNode->Find("Width")->GetValue().ToFloat();
  if(aNode->Find("Height"))
    mSize.y = aNode->Find("Height")->GetValue().ToFloat();
  if(aNode->Find("Primary"))
    mPrimary = aNode->Find("Primary")->GetValue().ToBool();
  
  delete mFramebuffer;
  
  #ifdef SHADER_COMPATIBLE
  mFramebuffer = new GLFramebuffer(mSize.x, mSize.y);
  #endif
  
  mFramebuffer->Generate(mManager);
}
  
// Statics
/**
 * @brief Serialize to LUA
 */
void Camera::SerializeLUA()
{
  SLB::Class<Camera>("Camera")
    .inherits<Component>()
    .set("GetSize", &Camera::GetSize)
    .set("SetSize", &Camera::SetSize)
    .set("GetOffset", &Camera::GetOffset)
    .set("SetOffset", &Camera::SetOffset);
}
