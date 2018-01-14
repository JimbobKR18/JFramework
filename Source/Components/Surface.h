#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"
#include "TextureCoordinates.h"
#include "MathExt.h"
#include "SurfaceProperty.h"

class GraphicsManager;

enum ScrollType
{
  VERTICAL = 0,
  HORIZONTAL
};

enum Viewspace
{
  VIEW_RELATIVE_TO_CAMERA = 0,
  VIEW_ABSOLUTE,
  VIEW_PERCENTAGE_OF_CAMERA
};

struct ScrollInfo
{
  ScrollType mType;
  Vector3 mGoalSize;
};

class Surface : public Component
{
public:
  typedef std::vector<ScrollInfo> ScrollInfoContainer;
  typedef ScrollInfoContainer::const_iterator ScrollInfoIT;
  typedef std::vector<SurfaceProperty*> PropertyContainer;
  typedef PropertyContainer::iterator PropertyContainerIt;
  typedef PropertyContainer::const_iterator PropertyContainerConstIt;
  
private:
  TextureCoordinates* mTexCoord;
  GraphicsManager*    mManager;
  Viewspace           mViewmode;
  Vector3             mTextureSize;
  Vector4             mColor;
  HashString          mFileName;
  bool                mNoRender;
  
  ScrollInfoContainer mScrollInfo;
  PropertyContainer   mProperties;
  
  static int const sUID;
public:
  Surface();
  Surface(GraphicsManager *aManager);
  virtual ~Surface();

  // Getters
  GraphicsManager*    GetManager() const { return mManager; }
  Viewspace           GetViewMode() const { return mViewmode; }
  Vector4&            GetColor() { return mColor; }
  HashString          GetFileName() const { return mFileName; }
  int                 GetCurrentAnimation() const { if(mTexCoord) return mTexCoord->GetCurrentAnimation(); else return 0; }
  int                 GetCurrentFrame() const { if(mTexCoord) return mTexCoord->GetCurrentFrame(); else return 0; }

  // Setters
  void                SetViewMode(Viewspace const& aViewmode) { mViewmode = aViewmode; }
  void                SetTextureSize(Vector3 const& aTextureSize) { mTextureSize = aTextureSize; }
  void                SetColor(Vector4 const& aColor) { mColor = aColor; }
  void                SetFileName(HashString const& aFileName) { mFileName = aFileName; }
  
  // Textures and Shaders
  virtual void        LoadImage(HashString const &aName);
  virtual Vector3     LoadText(HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth);
  virtual void        LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  
  // Texture Coordinates
  TextureCoordinates* GetTextureData() const;
  void                SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const &aNumFrames, std::vector<std::vector<float>> const &aAnimationSpeeds);
  void                SetAnimated(bool aAnimated);
  void                SetAnimation(int aAnimation, bool aRunOnce = false);
  void                SetCurrentFrame(int aFrame);
  void                SetFrameByID(int aFrameID);
  void                SetAnimationSpeed(float aAnimationSpeed);
  void                FinishAnimation();
  bool                CurrentAnimationCompleted();
  
  // Batching
  virtual unsigned    GetTextureID() const { assert(!"Cannot call GetTextureID() on a raw Surface."); return 0; }
  virtual unsigned    GetProgramID() const { assert(!"Cannot call GetProgramID() on a raw Surface."); return 0; }
  
  // Scrolling
  void                CreateScrollEffect(ScrollType const& aScrollType, Vector3 const& aGoalSize);
  
  // Properties
  PropertyContainer const&  GetProperties() const;
  void                      AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  void                      ClearProperties();
  
  // Derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(Parser &aParser);
  virtual void        Deserialize(ParserNode *aNode);
  static void         SerializeLUA();
  static int          GetUID() {return sUID;}
};

#endif
