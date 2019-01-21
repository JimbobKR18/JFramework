#include "Screen.h"
#include "Transform.h"
#include "ZRenderSorter.h"
#include "BatchRenderSorter.h"
#include "SystemProperties.h"
#include "ShapeMath.h"

Screen::Screen() : mOwner(nullptr), mWidth(0), mHeight(0), mFullScreen(false),  
  mBatchRenderSorter(nullptr), mDepthRenderSorter(nullptr), mUIRenderSorter(nullptr)
{
}

Screen::Screen(GraphicsManager *aOwner, int aW, int aH, bool aFullScreen) : 
  mOwner(aOwner), mWidth(aW), mHeight(aH), mFullScreen(aFullScreen)
{
  mDepthRenderSorter = new ZRenderSorter();
  mUIRenderSorter = new ZRenderSorter();
  mBatchRenderSorter = new BatchRenderSorter();
}

Screen::~Screen()
{
  delete mDepthRenderSorter;
  delete mUIRenderSorter;
  delete mBatchRenderSorter;
}

GraphicsManager* Screen::GetOwner() const
{
  return mOwner;
}

int Screen::GetWidth() const
{
  return mWidth;
}

void Screen::SetWidth(int const aWidth)
{
  mWidth = aWidth;
}

int Screen::GetHeight() const
{
  return mHeight;
}

void Screen::SetHeight(int const aHeight)
{
  mHeight = aHeight;
}

bool Screen::IsFullScreen() const
{
  return mFullScreen;
}

void Screen::SetFullScreen(bool const aFullScreen)
{
  mFullScreen = aFullScreen;
}

ScreenRenderSorter* Screen::GetBatchRenderSorter()
{
  return mBatchRenderSorter;
}

/**
 * @brief Redefine behavior of how objects should be rendered.
 * @param aBatchRenderSorter
 */
void Screen::SetBatchRenderSorter(ScreenRenderSorter *aBatchRenderSorter)
{
  if(mBatchRenderSorter)
    delete mBatchRenderSorter;
    
  mBatchRenderSorter = aBatchRenderSorter;
}

ScreenRenderSorter* Screen::GetDepthRenderSorter()
{
  return mDepthRenderSorter;
}

/**
 * @brief Redefine behavior of how objects should be rendered.
 * @param aDepthRenderSorter
 */
void Screen::SetDepthRenderSorter(ScreenRenderSorter *aDepthRenderSorter)
{
  if(mDepthRenderSorter)
    delete mDepthRenderSorter;
    
  mDepthRenderSorter = aDepthRenderSorter;
}

ScreenRenderSorter* Screen::GetUIRenderSorter()
{
  return mUIRenderSorter;
}

/**
 * @brief Redefine behavior of how objects should be rendered.
 * @param aUIRenderSorter
 */
void Screen::SetUIRenderSorter(ScreenRenderSorter *aUIRenderSorter)
{
  if(mDepthRenderSorter)
    delete mUIRenderSorter;
    
  mUIRenderSorter = aUIRenderSorter;
}

/**
 * @brief Per camera, prune out objects not in view.
 * @param aObjects
 * @param aCameras
 */
std::unordered_map<Camera*, std::map<int, std::vector<Surface*>>> Screen::PruneObjects(Tree *aObjects,
  std::unordered_set<Surface*> const &aMovingObjects, std::unordered_set<Camera*> const &aCameras)
{
  // Identity matrix
  Matrix33 identityMatrix = Matrix33();
  
  // Cardinal axes
  Vector3 axes[3];
  axes[0] = Vector3(1, 0, 0);
  axes[1] = Vector3(0, 1, 0);
  axes[2] = Vector3(0, 0, 1);
  
  // Return value
  std::unordered_map<Camera*, std::map<int, std::vector<Surface*>>> ret;
  
  // Draw each object
  // NOTE: The objects are sorted by texture id
  std::unordered_set<Camera*>::const_iterator cameraEnd = aCameras.end();
  for(std::unordered_set<Camera*>::const_iterator it = aCameras.begin(); it != cameraEnd; ++it)
  {
    // Camera position and size
    Camera* camera = *it;
    Transform *cameraTransform = camera->GetOwner()->GET<Transform>();
    Vector3 cameraPosition = cameraTransform->GetPosition() + camera->GetOffset();
    Vector3 cameraSize = camera->GetSize();
    if(cameraTransform->GetScale().length() < 1.4)
      cameraSize = camera->GetSize().Divide(cameraTransform->GetScale());
    
    Vector3 cameraHalfSize = cameraSize / 2.0f;
    Vector3 cameraMin = cameraPosition - cameraHalfSize;
    Vector3 cameraMax = cameraPosition + cameraHalfSize;
    Matrix33 viewMatrix = cameraTransform->GetFinalTransform();
    
    // Must scale, rotate, then translate camera offset
    Vector3 cameraDiff = (viewMatrix * cameraPosition) - cameraHalfSize;
    
    // Camera axes
    Vector3 cameraAxes[3];
    cameraAxes[0] = cameraTransform->GetHierarchicalRotation() * axes[0];
    cameraAxes[1] = cameraTransform->GetHierarchicalRotation() * axes[1];
    cameraAxes[2] = cameraTransform->GetHierarchicalRotation() * axes[2];
    
    // Cull terrain from data structure, whatever makes it is in.
    std::unordered_set<Surface*> staticSurfaces = aObjects->Query(cameraMin, cameraMax);
    std::unordered_set<Surface*>::const_iterator staticSurfacesEnd = staticSurfaces.end();
    for(std::unordered_set<Surface*>::const_iterator it2 = staticSurfaces.begin(); it2 != staticSurfacesEnd; ++it2)
    {
      Surface *surface = *it2;
      
      // Optimization
      if(ret[camera].find(surface->GetLayer()) == ret[camera].end())
      {
        ret[camera][surface->GetLayer()].reserve(staticSurfaces.size());
      }
      
      // No validation
      ret[camera][surface->GetLayer()].push_back(surface);
    }
    
    // Frustum cull objects that can move (non terrain data)
    std::unordered_set<Surface*>::const_iterator objectsEnd = aMovingObjects.end();
    for(std::unordered_set<Surface*>::const_iterator it2 = aMovingObjects.begin(); it2 != objectsEnd; ++it2)
    {
      // Get the surface details
      Surface *surface = *it2;
      GameObject *owner = surface->GetOwner();
      Transform *transform = owner->GET<Transform>();
      Viewspace viewSpace = surface->GetViewMode();
      Vector3 cameraTranslation;
      
      if(surface->GetNoRender())
      {
        continue;
      }
      
      // Optimization
      if(ret[camera].find(surface->GetLayer()) == ret[camera].end())
      {
        ret[camera][surface->GetLayer()].reserve(aMovingObjects.size());
      }
      
      // Camera translation
      Matrix33 cameraMatrix;
      if(surface->GetViewMode() == VIEW_ABSOLUTE)
      {
        cameraTranslation = cameraDiff;
        cameraMatrix = viewMatrix;
      }
      else // Menu objects do not rotate with camera.
      {
        cameraMatrix = identityMatrix;
      }
    
      // Get transforms in local and world space.
      Matrix33 modelTransform = transform->GetHierarchicalRotation() * Matrix33(transform->GetHierarchicalScale());
      Vector3 position = transform->GetHierarchicalPosition();
      TextureCoordinates *texCoord = surface->GetTextureData();
      Vector3 &size = transform->GetSize();
      
      // If transform is a percentage of screen, convert.
      if((*it2)->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = cameraSize.x * position.x;
        position.y = cameraSize.y * position.y;
      }

      // Move object based on its alignment
      AlignmentHelper(transform, size, position, transform->GetScale());
      
      // OBB test
      Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(cameraPosition, position, axes, transform->GetHierarchicalScale().Multiply(size));
      Vector3 diff = closestPoint - cameraPosition;
      
      bool added = true;
      for(int i = 0; i < 2; ++i)
      {
        float compare = cameraSize[i];
        float d = diff.Dot(cameraAxes[i]);
        if(fabs(d) > compare)
        {
          added = false;
          break;
        }
      }
      
      if(added)
        ret[camera][surface->GetLayer()].push_back(surface);
    }
  }
  return ret;
}

/**
 * @brief Sorts objects to be rendered. (Assumes that objects of same texture ID are at the same z depth.)
 * @param aObjects
 */
void Screen::SortObjects(std::vector<Surface*> &aObjects)
{
  //mBatchRenderSorter->SortPredicate(aObjects);
  mDepthRenderSorter->SortPredicate(aObjects);
}

/**
 * @brief Sorts objects to be rendered. (Assumes that objects of same texture ID are at the same z depth.)
 * @param aObjects
 */
void Screen::SortUI(std::vector<Surface*> &aObjects)
{
  mUIRenderSorter->SortPredicate(aObjects);
}

/**
 * @brief Align objects
 * @param aTransform Transform of object
 * @param aSize Object size
 * @param aPosition Object position
 * @param aScale Object scale
 */
void Screen::AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition, Vector3 &aScale)
{
  X_ALIGNMENT xAlign = aTransform->GetXAlignment();
  Y_ALIGNMENT yAlign = aTransform->GetYAlignment();
  Z_ALIGNMENT zAlign = aTransform->GetZAlignment();

  switch(xAlign)
  {
  case X_ALIGN_LEFT:
    aPosition.x += aSize.x * aScale.x;
    break;
  case X_ALIGN_RIGHT:
    aPosition.x -= aSize.x * aScale.x;
    break;
  default:
    break;
  }

  switch(yAlign)
  {
  case Y_ALIGN_TOP:
    aPosition.y += aSize.y * aScale.y;
    break;
  case Y_ALIGN_BOTTOM:
    aPosition.y -= aSize.y * aScale.y;
    break;
  default:
    break;
  }

  switch(zAlign)
  {
  case Z_ALIGN_FRONT:
    aPosition.z += aSize.z * aScale.z;
    break;
  case Z_ALIGN_BACK:
    aPosition.z -= aSize.z * aScale.z;
    break;
  default:
    break;
  }
}
