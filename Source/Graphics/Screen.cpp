#include "Screen.h"
#include "Transform.h"
#include "ZRenderSorter.h"
#include "SystemProperties.h"
#include "ShapeMath.h"

Screen::Screen() : mOwner(nullptr), mWidth(0), mHeight(0), mFullScreen(false),  
  mDepthRenderSorter(nullptr), mUIRenderSorter(nullptr)
{
}

Screen::Screen(GraphicsManager *aOwner, int aW, int aH, bool aFullScreen) : 
  mOwner(aOwner), mWidth(aW), mHeight(aH), mFullScreen(aFullScreen)
{
  mDepthRenderSorter = new ZRenderSorter();
  mUIRenderSorter = new ZRenderSorter();
}

Screen::~Screen()
{
  delete mDepthRenderSorter;
  delete mUIRenderSorter;
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
std::unordered_map<Camera*, std::map<int, std::vector<Renderable*>>> Screen::PruneObjects(Tree *aObjects,
  std::unordered_set<Renderable*> const &aMovingObjects, std::unordered_set<Camera*> const &aCameras)
{
  // Identity matrix
  Matrix33 identityMatrix = Matrix33();
  
  // Cardinal axes
  Vector3 axes[3];
  axes[0] = Vector3(1, 0, 0);
  axes[1] = Vector3(0, 1, 0);
  axes[2] = Vector3(0, 0, 1);
  
  // Return value
  std::unordered_map<Camera*, std::map<int, std::vector<Renderable*>>> ret;
  
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
    std::unordered_set<Renderable*> staticRenderables = aObjects->Query(cameraMin, cameraMax);
    std::unordered_set<Renderable*>::const_iterator staticRenderablesEnd = staticRenderables.end();
    for(std::unordered_set<Renderable*>::const_iterator it2 = staticRenderables.begin(); it2 != staticRenderablesEnd; ++it2)
    {
      Renderable *renderable = *it2;
      
      // Optimization
      if(ret[camera].find(renderable->GetLayer()) == ret[camera].end())
      {
        ret[camera][renderable->GetLayer()].reserve(staticRenderables.size());
      }
      
      // No validation
      ret[camera][renderable->GetLayer()].push_back(renderable);
    }
    
    // Frustum cull objects that can move (non terrain data)
    std::unordered_set<Renderable*>::const_iterator objectsEnd = aMovingObjects.end();
    for(std::unordered_set<Renderable*>::const_iterator it2 = aMovingObjects.begin(); it2 != objectsEnd; ++it2)
    {
      // Get the renderable details
      Renderable *renderable = *it2;
      GameObject *owner = renderable->GetOwner();
      Transform *transform = owner->GET<Transform>();
      Viewspace viewSpace = renderable->GetViewMode();
      Vector3 cameraTranslation;
      
      if(renderable->GetNoRender())
      {
        continue;
      }
      
      // Optimization
      if(ret[camera].find(renderable->GetLayer()) == ret[camera].end())
      {
        ret[camera][renderable->GetLayer()].reserve(aMovingObjects.size());
      }
      
      // Camera translation
      Matrix33 cameraMatrix;
      if(renderable->GetViewMode() == VIEW_ABSOLUTE)
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
      Vector3 &size = transform->GetSize();
      
      // If transform is a percentage of screen, convert.
      if(renderable->GetViewMode() == VIEW_PERCENTAGE_OF_CAMERA)
      {
        position.x = cameraSize.x * position.x;
        position.y = cameraSize.y * position.y;
      }

      // Move object based on its alignment
      Renderable::AlignmentHelper(transform, size, position, transform->GetScale());
      
      // OBB test
      Vector3 closestPoint = ShapeMath::ClosestPointPointOBB(cameraPosition, position, axes, transform->GetHierarchicalScale().Multiply(size));
      Vector3 diff = closestPoint - cameraPosition;
      if(renderable->GetViewMode() != VIEW_ABSOLUTE)
      {
        diff = closestPoint;
      }
      
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
        ret[camera][renderable->GetLayer()].push_back(renderable);
    }
  }
  return ret;
}

/**
 * @brief Sorts objects to be rendered. (Assumes that objects of same texture ID are at the same z depth.)
 * @param aObjects
 */
void Screen::SortObjects(std::vector<Renderable*> &aObjects)
{
  mDepthRenderSorter->SortPredicate(aObjects);
}

/**
 * @brief Sorts objects to be rendered. (Assumes that objects of same texture ID are at the same z depth.)
 * @param aObjects
 */
void Screen::SortUI(std::vector<Renderable*> &aObjects)
{
  mUIRenderSorter->SortPredicate(aObjects);
}
