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

enum TextRenderStyle
{
  DEFAULT_RENDER_STYLE = 0,
  SMOOTH_RENDER_STYLE,
  CHARACTER_BY_CHARACTER_STYLE
};

struct ScrollInfo
{
  ScrollType mType;
  Vector3 mGoalSize;
  
  ScrollInfo() : mType(ScrollType::VERTICAL), mGoalSize() {}
  ScrollInfo(ScrollInfo const &aScrollInfo) : mType(aScrollInfo.mType),
    mGoalSize(aScrollInfo.mGoalSize) {}
  virtual ~ScrollInfo() {}
};

class Surface : public Component
{
public:
  typedef std::vector<ScrollInfo> ScrollInfoContainer;
  typedef ScrollInfoContainer::const_iterator ScrollInfoIT;
  
private:
  // Base
  TextureCoordinates* mTexCoord;
  GraphicsManager*    mManager;
  Viewspace           mViewmode;
  Vector3             mTextureSize;
  Vector4             mPrimaryColor;
  bool                mNoRender;
  HashString          mMinFilter;
  HashString          mMagFilter;
  int                 mLayer;
  
  // Default
  HashString          mFileName;
  
  // Text
  HashString          mText;
  HashString          mFontName;
  int                 mFontSize;
  int                 mMaxTextWidth;
  Vector4             mSecondaryColor;
  Vector3             mOriginalSize;
  TextRenderStyle     mTextRenderStyle;
  
  // Etc.
  ScrollInfoContainer mScrollInfo;
  PropertyContainer   mProperties;
  bool                mUIElement;
  
  static int const sUID;
public:
  Surface();
  Surface(Surface const &aSurface);
  Surface(GraphicsManager *aManager);
  virtual ~Surface();

  // Getters
  GraphicsManager*    GetManager() const { return mManager; }
  Viewspace           GetViewMode() const { return mViewmode; }
  Vector4&            GetColor() { return mPrimaryColor; }
  bool                GetNoRender() { return mNoRender; }
  HashString          GetFileName() const { return mFileName; }
  HashString          GetText() const { return mText; }
  HashString          GetFontName() const { return mFontName; }
  int                 GetFontSize() const { return mFontSize; }
  int                 GetMaxTextWidth() const { return mMaxTextWidth; }
  Vector4             GetSecondaryColor() const { return mSecondaryColor; }
  Vector3             GetOriginalSize() const { return mOriginalSize; }
  TextRenderStyle     GetTextRenderStyle() const { return mTextRenderStyle; }
  int                 GetCurrentAnimation() const { if(mTexCoord) return mTexCoord->GetCurrentAnimation(); else return 0; }
  int                 GetCurrentFrame() const { if(mTexCoord) return mTexCoord->GetCurrentFrame(); else return 0; }
  HashString          GetMinFilter() const { return mMinFilter; }
  HashString          GetMagFilter() const { return mMagFilter; }
  int                 GetLayer() const { return mLayer; }
  Vector3             GetTextureSize() const { return mTextureSize; }
  bool                GetUIElement() const { return mUIElement; }

  // Setters
  void                SetViewMode(Viewspace const& aViewmode) { mViewmode = aViewmode; }
  void                SetTextureSize(Vector3 const& aTextureSize) { mTextureSize = aTextureSize; }
  void                SetColor(Vector4 const& aColor) { mPrimaryColor = aColor; }
  void                SetNoRender(bool const aNoRender) { mNoRender = aNoRender; }
  void                SetText(HashString const &aText) { mText = aText; }
  void                SetFontName(HashString const &aFontName) { mFontName = aFontName; }
  void                SetFontSize(int const aFontSize) { mFontSize = aFontSize; }
  void                SetMaxTextWidth(int const aMaxTextWidth) { mMaxTextWidth = aMaxTextWidth; }
  void                SetSecondaryColor(Vector4 const &aSecondaryColor) { mSecondaryColor = aSecondaryColor; }
  void                SetOriginalSize(Vector3 const &aOriginalSize) { mOriginalSize = aOriginalSize; }
  void                SetFileName(HashString const& aFileName) { mFileName = aFileName; }
  void                SetTextRenderStyle(TextRenderStyle const& aTextRenderStyle) { mTextRenderStyle = aTextRenderStyle; }
  void                SetMinFilter(HashString const &aMinFilter) { mMinFilter = aMinFilter; }
  void                SetMagFilter(HashString const &aMagFilter) { mMagFilter = aMagFilter; }
  void                SetLayer(int const aLayer) { mLayer = aLayer; }
  void                SetUIElement(bool const aUIElement) { mUIElement = aUIElement; }
  
  // Textures and Shaders
  virtual void        LoadImage(HashString const &aName);
  virtual void        LoadText(HashString const &aText);
  virtual void        LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  
  // Texture Coordinates
  TextureCoordinates* GetTextureData() const;
  void                SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const &aNumFrames, std::vector<std::vector<float>> const &aAnimationSpeeds);
  void                SetAnimated(bool aAnimated);
  void                SetAnimation(int aAnimation, int aBehavior = TextureCoordinateBehavior::CONTINUOUS);
  void                SetCurrentFrame(int aFrame);
  void                SetFrameByID(int aFrameID);
  void                SetAnimationSpeed(float aAnimationSpeed);
  void                FinishAnimation();
  bool                CurrentAnimationCompleted();
  
  // Batching
  bool                SurfacePropertiesEquals(Surface const *aSurface) const;
  virtual unsigned    GetTextureID() const { assert(!"Cannot call GetTextureID() on a raw Surface."); return 0; }
  virtual unsigned    GetProgramID() const { assert(!"Cannot call GetProgramID() on a raw Surface."); return 0; }
  
  // Scrolling
  void                CreateScrollEffect(ScrollType const& aScrollType, Vector3 const& aGoalSize);
  
  // Properties
  PropertyContainer const&  GetProperties() const;
  void                      AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  void                      AddOrEditPropertyWithId(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue, int const aId);
  void                      ClearProperties();
  
  // Derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(ParserNode *aNode);
  virtual void        Deserialize(ParserNode *aNode);
  virtual Component*  Clone(GameObject *aNewOwner) const;
  static void         SerializeLUA();
  static int          GetUID() {return sUID;}
};

#endif
