#ifndef __JFramework__Camera__
#define __JFramework__Camera__

#include "Common.h"
#include "MathExt.h"
#include "Component.h"
#include "Pipeline.h"

class GraphicsManager;

class Camera : public Component
{
private:
  Vector3 mSize;
  Vector3 mOffset;
  bool mPrimary;
  GraphicsManager *mManager;
  FrameLayerContainer mFramebuffers;
  HashString mPipelineName;

  static int const  sUID;
  
public:
  Camera();
  Camera(GraphicsManager *aManager);
  Camera(Camera const &aCamera);
  virtual ~Camera();
  
  // Methods
  Vector3 GetSize() const;
  void SetSize(Vector3 const &aSize);
  Vector3& GetOffset();
  void SetOffset(Vector3 const &aOffset);
  bool GetPrimary() const;
  void SetPrimary(bool aPrimary);
  Framebuffer* GetFramebuffer(int const aLayer);
  FrameLayerContainer const &GetFramebuffers() const;
  HashString GetPipelineName() const;
  void SetPipelineName(HashString const &aPipelineName);
  
  // Virtuals derived from Component
  virtual void Update();
  virtual void SendMessage(Message const& aMessage);
  virtual void ReceiveMessage(Message const& aMessage);
  virtual void Serialize(ParserNode *aNode);
  virtual void Deserialize(ParserNode *aNode);
  virtual Component* Clone(GameObject *aNewOwner) const;
  
  // Statics
  static void SerializeLUA();
  static int GetUID() {return sUID;}
};

#endif // __JFramework__Camera__
