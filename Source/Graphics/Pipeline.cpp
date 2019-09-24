#include "Pipeline.h"
#include "GraphicsManager.h"
#include "SystemProperties.h"

#if !defined(IOS) && !defined(ANDROID)
  #define SHADER_COMPATIBLE
  #include "GLFramebuffer.h"
#endif

//--------------------------------
// Pass
//--------------------------------
Pass::Pass(GraphicsManager *aManager, HashString const &aVertexShaderName, HashString const &aFragmentShaderName, Vector4 const &aClearColor) : 
  mDefaultFrameBufferID(0), mVertexShaderName(aVertexShaderName), mFragmentShaderName(aFragmentShaderName)
{
  #ifdef SHADER_COMPATIBLE
    mOutput = new GLFramebuffer(SystemProperties::GetRenderWidth(), SystemProperties::GetRenderHeight(), 
      SystemProperties::GetMinFilter(), SystemProperties::GetMagFilter());
    mOutput->SetShaders(aManager, mVertexShaderName, mFragmentShaderName);
    mOutput->SetClearColor(aClearColor);
    mOutput->Generate(aManager);
    
    mInput = new GLFramebuffer(SystemProperties::GetRenderWidth(), SystemProperties::GetRenderHeight(), 
      SystemProperties::GetMinFilter(), SystemProperties::GetMagFilter());
    mInput->SetShaders(aManager, mVertexShaderName, mFragmentShaderName);
    mInput->SetClearColor(aClearColor);
    mInput->Generate(aManager);
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mDefaultFrameBufferID);
  #endif
}

Pass::~Pass()
{
  delete mOutput;
  delete mInput;
}

/**
 * @brief Run pass using inputs
 * @param aInputs Input framebuffers
 */
void Pass::Run(GraphicsManager *aManager, std::vector<Framebuffer*> const &aInputs)
{
  std::vector<int> buffers;
  for(std::vector<Framebuffer*>::const_iterator it = aInputs.begin(); it != aInputs.end(); ++it)
  {
    buffers.push_back((*it)->GetTextureID());
  }
  
  mOutput->Bind();
  mInput->SetInputTextures(buffers);
  mInput->Draw(aManager, SystemProperties::GetRenderWidth(), SystemProperties::GetRenderHeight(), 
    SystemProperties::GetScreenWidth(), SystemProperties::GetScreenHeight(), false);
  mOutput->Unbind(mDefaultFrameBufferID);
}


//--------------------------------
// Pipeline
//--------------------------------
Pipeline::Pipeline(HashString const &aName) : mName(aName), mPasses()
{
}

Pipeline::~Pipeline()
{
  for(PassContainerIT it = mPasses.begin(); it != mPasses.end(); ++it)
  {
    PassStep &step = *it;
    for(PassStep::iterator it2 = step.begin(); it2 != step.end(); ++it2)
    {
      delete *it2;
    }
    step.clear();
  }
  mPasses.clear();
}

/**
 * @brief Add pass
 * @param aIndex Index to add step
 * @param aPass Pass to add
 */
void Pipeline::AddPass(int aIndex, Pass* aPass)
{
  if(aIndex >= mPasses.size())
    mPasses.resize(aIndex + 1);
  
  mPasses[aIndex].push_back(aPass);
}

/**
 * @brief Run through and produce image
 * @param aLayers Input layers, contain image ids
 * @return Finalized output
 */
Framebuffer *Pipeline::Run(GraphicsManager *aManager, FrameLayerContainer const &aLayers)
{
  std::vector<Framebuffer*> current;
  for(FrameLayerContainerConstIT it = aLayers.begin(); it != aLayers.end(); ++it)
  {
    current.push_back(it->second);
  }
  
  Framebuffer* output;
  for(PassContainerIT it = mPasses.begin(); it != mPasses.end(); ++it)
  {
    std::vector<Framebuffer*> temp;
    PassStep &step = *it;
    for(PassStep::iterator it2 = step.begin(); it2 != step.end(); ++it2)
    {
      Pass *pass = *it2;
      pass->Run(aManager, current);
      temp.push_back(pass->mOutput);
    }
    current = temp;
    output = current[0];
  }
  return output;
}
  