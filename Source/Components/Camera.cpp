#include "Camera.h"
#include "GLFramebuffer.h"
#include "SystemProperties.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "Transform.h"

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
  mOffset(), mPrimary(false), mManager(aManager), mFramebuffers(), mPipelineName("Default")
{
}

Camera::Camera(Camera const &aCamera) : Component(Camera::sUID), mSize(aCamera.mSize),
  mOffset(aCamera.mOffset), mPrimary(false), mManager(aCamera.mManager),
  mFramebuffers(), mPipelineName("Default")
{
}

Camera::~Camera()
{
  for(FrameLayerContainerIT it = mFramebuffers.begin(); it != mFramebuffers.end(); ++it)
    delete it->second;
  mFramebuffers.clear();
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
 * @param aLayer The layer to retrieve
 * @return Frame buffer
 */
Framebuffer* Camera::GetFramebuffer(int const aLayer)
{
  if(mFramebuffers.find(aLayer) == mFramebuffers.end())
  {
#ifdef SHADER_COMPATIBLE
    mFramebuffers[aLayer] = new GLFramebuffer(mSize.x, mSize.y, SystemProperties::GetMinFilter(), SystemProperties::GetMagFilter());
    mFramebuffers[aLayer]->SetShaders(mManager, SystemProperties::GetFramebufferVertexShaderName(), SystemProperties::GetFramebufferFragmentShaderName());
    mFramebuffers[aLayer]->Generate(mManager);
#endif
  }
  
  return mFramebuffers[aLayer];
}

/**
 * @brief Get all framebuffers being drawn to
 * @return Framebuffers
 */
FrameLayerContainer const &Camera::GetFramebuffers() const
{
  return mFramebuffers;
}

/**
 * @brief Get name of pipeline
 * @return Pipeline
 */
HashString Camera::GetPipelineName() const
{
  return mPipelineName;
}

/**
 * @brief Set name of pipeline
 * @param aPipelineName Pipeline name
 */
void Camera::SetPipelineName(HashString const &aPipelineName)
{
  mPipelineName = aPipelineName;
}

// Virtuals derived from Component
/**
 * @brief Does nothing
 */
void Camera::Update()
{
  Transform *cameraTransform = GetOwner()->GET<Transform>();
  cameraTransform->SetSize(mSize / 2.0f);
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
  object->Place("Pipeline", mPipelineName);
  
  if(mManager->GetPrimaryCamera() == this)
    object->Place("Primary", Common::BoolToString(true));
}

/**
 * @brief Deserialize from file
 * @param aNode Node to read from
 */
void Camera::Deserialize(ParserNode *aNode)
{
  HashString vertexShaderFileName = SystemProperties::GetFramebufferVertexShaderName();
  HashString fragmentShaderFileName = SystemProperties::GetFramebufferFragmentShaderName();
  
  if(aNode->Find("Width"))
    mSize.x = aNode->Find("Width")->GetValue().ToFloat();
  if(aNode->Find("Height"))
    mSize.y = aNode->Find("Height")->GetValue().ToFloat();
  if(aNode->Find("Primary"))
    mPrimary = aNode->Find("Primary")->GetValue().ToBool();
  if(aNode->Find("Pipeline"))
    mPipelineName = aNode->Find("Pipeline")->GetValue();
}

/**
 * @brief Clone Camera
 * @param aNewOwner The new owner
 * @return Cloned Camera
 */
Component* Camera::Clone(GameObject *aNewOwner) const
{
  return new Camera(*this);
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
