#ifndef __JFramework_Framebuffer_h_
#define __JFramework_Framebuffer_h_

#include "Common.h"
#include "SurfaceProperty.h"

class GraphicsManager;

class Framebuffer
{
private:
  PropertyContainer mProperties;

public:
  Framebuffer();
  virtual ~Framebuffer();
  
  // PROPERTIES
  PropertyContainer const& GetProperties() const;
  void AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue);
  void ClearProperties();
  
  // VIRTUAL GETTERS
  virtual int GetTextureID() const = 0;
  
  // VIRTUAL SETTERS
  virtual void SetShaders(GraphicsManager *aManager, HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename) = 0;
  
  // VIRTUAL METHODS
  virtual void Generate(GraphicsManager *aManager) = 0;
  virtual void Bind() = 0;
  virtual void Unbind(int aDefaultFramebuffer) = 0;
  virtual void Draw(int aDefaultWidth, int aDefaultHeight, int aScreenWidth, int aScreenHeight, bool aFullScreen) = 0;
  virtual void SetShaderProperties(bool aActive) = 0;
};

#endif //__JFramework_Framebuffer_h_