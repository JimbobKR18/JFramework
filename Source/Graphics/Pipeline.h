#ifndef PIPELINE_H
#define PIPELINE_H

#include "Common.h"
#include "Framebuffer.h"

class GraphicsManager;

struct Pass
{
  Framebuffer *mOutput;
  Framebuffer *mInput;
  int mDefaultFrameBufferID;
  HashString mVertexShaderName;
  HashString mFragmentShaderName;
  
  Pass(GraphicsManager *aManager, HashString const &aVertexShaderName, HashString const &aFragmentShaderName,
    Vector4 const &aClearColor);
  virtual ~Pass();
  
  void Run(std::vector<Framebuffer*> const &aInputs);
};

typedef std::vector<Pass*> PassStep;
typedef PassStep::iterator PassStepIT;
typedef PassStep::const_iterator PassStepConstIT;

typedef std::vector<PassStep> PassContainer;
typedef PassContainer::iterator PassContainerIT;
typedef PassContainer::const_iterator PassContainerConstIT;

typedef std::map<int, Framebuffer*> FrameLayerContainer;
typedef FrameLayerContainer::iterator FrameLayerContainerIT;
typedef FrameLayerContainer::const_iterator FrameLayerContainerConstIT;

class Pipeline
{
private:
  HashString mName;
  PassContainer mPasses;
  
public:
  Pipeline(HashString const &aName);
  virtual ~Pipeline();
  
  void AddPass(int aIndex, Pass* aPass);
  Framebuffer *Run(FrameLayerContainer const &aLayers);
};

#endif // PIPELINE_H
