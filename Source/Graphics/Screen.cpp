#include "Screen.h"
#include "Transform.h"
#include "ZRenderSorter.h"
#include "BatchRenderSorter.h"
#include "SystemProperties.h"

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
  
  // Return value
  std::unordered_map<Camera*, std::map<int, std::vector<Surface*>>> ret;
  
  // Points storage
  std::vector<Vector3> points;
  points.reserve(5);
  
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
        ++it2;
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
      AlignmentHelper(transform, size, position);
      
      // Get the basic coordinates for the quad
      Vector3 topLeft(-size.x, -size.y, 0);
      Vector3 topRight(size.x, -size.y, 0);
      Vector3 bottomRight(size.x, size.y, 0);
      Vector3 bottomLeft(-size.x, size.y, 0);
      
      // Model transform
      topLeft = cameraMatrix * ((modelTransform * topLeft) + position) - cameraTranslation;
      topRight = cameraMatrix * ((modelTransform * topRight) + position) - cameraTranslation;
      bottomLeft = cameraMatrix * ((modelTransform * bottomLeft) + position) - cameraTranslation;
      bottomRight = cameraMatrix * ((modelTransform * bottomRight) + position) - cameraTranslation;

      points.clear();
      points.push_back(topLeft);
      points.push_back(topRight);
      points.push_back(bottomLeft);
      points.push_back(bottomRight);
      points.push_back((cameraMatrix * position) - cameraTranslation);
      
      bool added = false;
      std::vector<Vector3>::const_iterator pointsEnd = points.end();
      for(std::vector<Vector3>::const_iterator it3 = points.begin(); it3 != pointsEnd; ++it3)
      {
        if(it3->x <= cameraSize.x && it3->x >= 0 &&
          it3->y <= cameraSize.y && it3->y >= 0) 
        {
          added = true;
          ret[camera][surface->GetLayer()].push_back(surface);
          break;
        }
      }
      if(added)
        continue;
      
      if((points[0].x <= 0 && points[1].x >= cameraSize.x) ||
        (points[1].x <= 0 && points[0].x >= cameraSize.x))
      {
        ret[camera][surface->GetLayer()].push_back(surface);
        continue;
      }
      if((points[0].y <= 0 && points[2].y >= cameraSize.y) ||
        (points[2].y <= 0 && points[0].y >= cameraSize.y))
      {
        ret[camera][surface->GetLayer()].push_back(surface);
        continue;
      }
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
 */
void Screen::AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition)
{
  X_ALIGNMENT xAlign = aTransform->GetXAlignment();
  Y_ALIGNMENT yAlign = aTransform->GetYAlignment();
  Z_ALIGNMENT zAlign = aTransform->GetZAlignment();

  switch(xAlign)
  {
  case X_ALIGN_LEFT:
    aPosition.x += aSize.x;
    break;
  case X_ALIGN_RIGHT:
    aPosition.x -= aSize.x;
    break;
  default:
    break;
  }

  switch(yAlign)
  {
  case Y_ALIGN_TOP:
    aPosition.y += aSize.y;
    break;
  case Y_ALIGN_BOTTOM:
    aPosition.y -= aSize.y;
    break;
  default:
    break;
  }

  switch(zAlign)
  {
  case Z_ALIGN_FRONT:
    aPosition.z += aSize.z;
    break;
  case Z_ALIGN_BACK:
    aPosition.z -= aSize.z;
    break;
  default:
    break;
  }
}
