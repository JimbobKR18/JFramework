#include "SystemProperties.h"
#include "LUATypes.h"

/* This class is meant to be read in from a file in the same location as
 * the executable. */
HashString SystemProperties::mAssetsDirectory = HashString();
HashString SystemProperties::mDefaultVertexShaderName = HashString();
HashString SystemProperties::mDefaultFragmentShaderName = HashString();
HashString SystemProperties::mFramebufferVertexShaderName = HashString();
HashString SystemProperties::mFramebufferFragmentShaderName = HashString();
HashString SystemProperties::mMagFilter = HashString();
HashString SystemProperties::mMinFilter = HashString();
HashString SystemProperties::mWrapModeS = HashString();
HashString SystemProperties::mWrapModeT = HashString();
bool SystemProperties::mFullscreen = false;
HashString SystemProperties::mGameTitle = HashString();
int SystemProperties::mScreenWidth = 0;
int SystemProperties::mScreenHeight = 0;
int SystemProperties::mRenderWidth = 0;
int SystemProperties::mRenderHeight = 0;
float SystemProperties::mRefreshRate = 0;
bool SystemProperties::mLockedFramerate = false;
bool SystemProperties::m2DCollisionOnly = false;
int SystemProperties::mPacketSize = 0;

SystemProperties::SystemProperties()
{
}

SystemProperties::~SystemProperties()
{
}

/**
 * @brief Get assets directory
 * @return Assets directory as a HashString.
 */
HashString const &SystemProperties::GetAssetsDirectory()
{
  return mAssetsDirectory;
}

/**
 * @brief Get default vertex shader file name.
 * @return Vertex shader file name as HashString.
 */
HashString const &SystemProperties::GetDefaultVertexShaderName()
{
  return mDefaultVertexShaderName;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetDefaultFragmentShaderName()
{
  return mDefaultFragmentShaderName;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetFramebufferVertexShaderName()
{
  return mFramebufferVertexShaderName;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetFramebufferFragmentShaderName()
{
  return mFramebufferFragmentShaderName;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetMagFilter()
{
  return mMagFilter;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetMinFilter()
{
  return mMinFilter;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetWrapModeS()
{
  return mWrapModeS;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetWrapModeT()
{
  return mWrapModeT;
}

/**
 * @brief 
 * @return 
 */
bool const SystemProperties::GetFullScreen()
{
  return mFullscreen;
}

/**
 * @brief 
 * @return 
 */
HashString const &SystemProperties::GetGameTitle()
{
  return mGameTitle;
}

/**
 * @brief 
 * @return 
 */
int const SystemProperties::GetScreenWidth()
{
  return mScreenWidth;
}

/**
 * @brief 
 * @return 
 */
int const SystemProperties::GetScreenHeight()
{
  return mScreenHeight;
}

/**
 * @brief 
 * @return 
 */
int const SystemProperties::GetRenderWidth()
{
  return mRenderWidth;
}

/**
 * @brief 
 * @return 
 */
int const SystemProperties::GetRenderHeight()
{
  return mRenderHeight;
}

/**
 * @brief 
 * @return 
 */
float const SystemProperties::GetRefreshRate()
{
  return mRefreshRate;
}

/**
 * @brief 
 * @return 
 */
bool const SystemProperties::GetLockedFramerate()
{
  return mLockedFramerate;
}

/**
 * @brief 
 * @return 
 */
bool const SystemProperties::Get2DCollisionOnly()
{
  return m2DCollisionOnly;
}

/**
 * @brief 
 * @return 
 */
int const SystemProperties::GetPacketSize()
{
  return mPacketSize;
}

/**
 * @brief Write out to file.
 */
void SystemProperties::Serialize()
{
  std::ofstream outfile("./SystemProperties.ini");
  outfile << "AssetsDirectory = " << mAssetsDirectory << std::endl;
  outfile << "DefaultVertexShaderFileName = " << mDefaultVertexShaderName << std::endl;
  outfile << "DefaultFragmentShaderFileName = " << mDefaultFragmentShaderName << std::endl;
  outfile << "FramebufferVertexShaderFileName = " << mFramebufferVertexShaderName << std::endl;
  outfile << "FramebufferFragmentShaderFileName = " << mFramebufferFragmentShaderName << std::endl;
  outfile << "MagFilter = " << mMagFilter << std::endl;
  outfile << "MinFilter = " << mMinFilter << std::endl;
  outfile << "WrapModeS = " << mWrapModeS << std::endl;
  outfile << "WrapModeT = " << mWrapModeT << std::endl;
  outfile << "FullScreen = " << mFullscreen << std::endl;
  outfile << "GameTitle = " << mGameTitle << std::endl;
  outfile << "ScreenWidth = " << mScreenWidth << std::endl;
  outfile << "ScreenHeight = " << mScreenHeight << std::endl;
  outfile << "RenderWidth = " << mRenderWidth << std::endl;
  outfile << "RenderHeight = " << mRenderHeight << std::endl;
  outfile << "RefreshRate = " << mRefreshRate << std::endl;
  outfile << "LockedFramerate = " << mLockedFramerate << std::endl;
  outfile << "2DCollisionOnly = " << m2DCollisionOnly << std::endl;
  outfile << "PacketSize = " << mPacketSize << std::endl;
  outfile.close();
}

/**
 * @brief Read in from file.
 */
void SystemProperties::Deserialize()
{
  std::ifstream infile("./SystemProperties.ini");
  if(!infile.good())
    assert(!"No system property file found, please place one in same directory as executable.");
    
  while(infile.good() && !infile.eof())
  {
    std::string key, value, empty;
    infile >> key;
    infile >> empty;
    infile >> value;
    
    if(key == "AssetsDirectory")
      mAssetsDirectory = value;
    else if(key == "DefaultVertexShaderFileName")
      mDefaultVertexShaderName = value;
    else if(key == "DefaultFragmentShaderFileName")
      mDefaultFragmentShaderName = value;
    else if(key == "FramebufferVertexShaderFileName")
      mFramebufferVertexShaderName = value;
    else if(key == "FramebufferFragmentShaderFileName")
      mFramebufferFragmentShaderName = value;
    else if(key == "MagFilter")
      mMagFilter = value;
    else if(key == "MinFilter")
      mMinFilter = value;
    else if(key == "WrapModeS")
      mWrapModeS = value;
    else if(key == "WrapModeT")
      mWrapModeT = value;
    else if(key == "FullScreen")
      mFullscreen = Common::StringToBool(value);
    else if(key == "GameTitle")
      mGameTitle = value;
    else if(key == "ScreenWidth")
      mScreenWidth = Common::StringToInt(value);
    else if(key == "ScreenHeight")
      mScreenHeight = Common::StringToInt(value);
    else if(key == "RenderWidth")
      mRenderWidth = Common::StringToInt(value);
    else if(key == "RenderHeight")
      mRenderHeight = Common::StringToInt(value);
    else if(key == "RefreshRate")
      mRefreshRate = Common::StringToFloat(value);
    else if(key == "LockedFramerate")
      mLockedFramerate = Common::StringToBool(value);
    else if(key == "2DCollisionOnly")
      m2DCollisionOnly = Common::StringToBool(value);
    else if(key == "PacketSize")
      mPacketSize = Common::StringToInt(value);
    else if(!key.empty())
    {
      DebugLogPrint("Invalid property %s found.\n", key.c_str());
      assert(!"Invalid property found.");
    }
  }
  infile.close();
}

void SystemProperties::SerializeLUA()
{
  SLB::Class<SystemProperties>("SystemProperties").set("GetAssetsDirectory", &SystemProperties::GetAssetsDirectory);
}
