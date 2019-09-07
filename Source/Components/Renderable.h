#ifndef __JFramework_Surface_h_
#define __JFramework_Surface_h_

#include "Component.h"
#include "TextureCoordinates.h"
#include "MathExt.h"
#include "RenderableProperty.h"
#include "Transform.h"

enum ScrollType
{
  VERTICAL = 0,
  HORIZONTAL
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
  Vector2 mGoalSize;
  
  ScrollInfo() : mType(ScrollType::VERTICAL), mGoalSize() {}
  ScrollInfo(ScrollInfo const &aScrollInfo) : mType(aScrollInfo.mType),
    mGoalSize(aScrollInfo.mGoalSize) {}
  virtual ~ScrollInfo() {}
};

enum Viewspace
{
  VIEW_RELATIVE_TO_CAMERA = 0,
  VIEW_ABSOLUTE,
  VIEW_PERCENTAGE_OF_CAMERA
};

struct VertexData
{
  Vector3 mPosition;
  Vector3 mVertex;
  Vector2 mTextureCoord;
  Vector4 mColor;
  Vector3 mNormal;
  
  VertexData() : mPosition(), mVertex(), mTextureCoord(), mColor(), mNormal() {}
  VertexData(Vector3 const &aPosition, Vector3 const &aVertex, Vector2 const &aTextureCoord, Vector4 const &aColor, Vector3 const &aNormal) :
    mPosition(aPosition), mVertex(aVertex), mTextureCoord(aTextureCoord), mColor(aColor), mNormal(aNormal) {}
  virtual ~VertexData() {}
};

typedef std::vector<VertexData> VertexContainer;
typedef VertexContainer::iterator VertexIT;
typedef VertexContainer::const_iterator VertexConstIT;

class GraphicsManager;
class Renderable : public Component
{
public:
  static unsigned const BACKGROUND = 1000;
  static unsigned const GEOMETRY = 2000;
  static unsigned const TRANSPARENT = 3000;
  static unsigned const OVERLAY = 4000;
  
  typedef std::vector<ScrollInfo> ScrollInfoContainer;
  typedef ScrollInfoContainer::const_iterator ScrollInfoIT;
  typedef std::vector<int> IsolatedRenderLayerContainer;
  typedef IsolatedRenderLayerContainer::const_iterator IsolatedRenderLayerIT;
  
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
  IsolatedRenderLayerContainer mIsolatedRenderLayers;
  
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
  
  static HashType const sUID;
public:
  Renderable();
  Renderable(Renderable const &aSurface);
  Renderable(GraphicsManager *aManager);
  virtual ~Renderable();

  // Getters
  GraphicsManager*    GetManager() const { return mManager; }
  Vector4&            GetColor() { return mPrimaryColor; }
  Vector4 const&      GetColor() const { return mPrimaryColor; }
  HashString const&   GetFileName() const { return mFileName; }
  HashString const&   GetText() const { return mText; }
  HashString const&   GetFontName() const { return mFontName; }
  int                 GetFontSize() const { return mFontSize; }
  int                 GetMaxTextWidth() const { return mMaxTextWidth; }
  Vector4 const&      GetSecondaryColor() const { return mSecondaryColor; }
  Vector3 const&      GetOriginalSize() const { return mOriginalSize; }
  TextRenderStyle     GetTextRenderStyle() const { return mTextRenderStyle; }
  int                 GetCurrentAnimation() const { if(mTexCoord) return mTexCoord->GetCurrentAnimation(); else return 0; }
  int                 GetCurrentFrame() const { if(mTexCoord) return mTexCoord->GetCurrentFrame(); else return 0; }
  HashString const&   GetMinFilter() const { return mMinFilter; }
  HashString const&   GetMagFilter() const { return mMagFilter; }
  Vector3 const&      GetTextureSize() const { return mTextureSize; }
  int                 GetLayer() const { return mLayer; }
  Viewspace           GetViewMode() const { return mViewmode; }
  bool                GetNoRender() const { return mNoRender; }
  bool                GetUIElement() const { return mUIElement; }
  IsolatedRenderLayerContainer const &GetIsolatedRenderLayers() const { return mIsolatedRenderLayers; }

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
  void                SetIsolatedRenderLayers(IsolatedRenderLayerContainer const &aIsolatedRenderLayers) { mIsolatedRenderLayers = aIsolatedRenderLayers; }
  
  // Textures and Shaders
  virtual void        LoadImage(HashString const &aName);
  virtual void        LoadText(HashString const &aText);
  virtual void        LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename);
  
  // Texture Coordinates
  TextureCoordinates* GetTextureData() const;
  void                SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const &aNumFrames, std::vector<std::vector<float>> const &aAnimationSpeeds);
  void                SetTextureCoordinateData(int const aNumColumns, int const aNumRows, std::vector<AnimationInfo*> const &aAnimations);
  void                SetAnimated(bool aAnimated);
  void                SetAnimation(int aAnimation, int aBehavior = TextureCoordinateBehavior::CONTINUOUS);
  void                SetCurrentFrame(int aFrame);
  void                SetFrameByID(int aFrameID);
  void                SetAnimationSpeed(float aAnimationSpeed);
  void                FinishAnimation();
  bool                CurrentAnimationCompleted();
  
  // Scrolling
  void                CreateScrollEffect(ScrollType const& aScrollType, Vector3 const& aGoalSize);
  
  // Properties
  virtual bool                RenderablePropertiesEquals(Renderable const *aRenderable) const;
  virtual PropertyContainer const&  GetProperties() const;
  virtual void                      AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  virtual void                      AddOrEditPropertyWithId(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue, HashString const &aId);
  virtual void                      ClearProperties();
  
  // Derive these
  virtual unsigned    GetTextureID() const { assert(!"Cannot call GetTextureID() on a raw Surface."); return 0; }
  virtual unsigned    GetProgramID() const { assert(!"Cannot call GetProgramID() on a raw Surface."); return 0; }
  virtual VertexContainer GetVertexData(Vector3 const &aPosition) const { assert(!"Cannot call GetVertexData() on a raw Surface."); return VertexContainer(); };
  
  // Derived from Component
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ReceiveMessage(Message const &aMessage);
  virtual void        Serialize(ParserNode *aNode);
  virtual void        Deserialize(ParserNode *aNode);
  virtual Component*  Clone(GameObject *aNewOwner) const;
  static void         SerializeLUA();
  static HashType     GetUID() {return sUID;}
  
  static void AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition, Vector3 &aScale);
};

#endif
