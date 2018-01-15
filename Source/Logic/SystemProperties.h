/*
 * SystemProperties.h
 *
 *  Created on: May 17, 2016
 *      Author: jimbob
 */

#ifndef __JFramework_SystemProperties_h_
#define __JFramework_SystemProperties_h_

#include "Common.h"

class SystemProperties
{
private:
  static HashString mAssetsDirectory;
  static HashString mDefaultVertexShaderName;
  static HashString mDefaultFragmentShaderName;
  static HashString mFramebufferVertexShaderName;
  static HashString mFramebufferFragmentShaderName;
  static HashString mMagFilter;
  static HashString mMinFilter;
  static HashString mWrapModeS;
  static HashString mWrapModeT;
  static bool mFullscreen;
  static HashString mGameTitle;
  static int mScreenWidth;
  static int mScreenHeight;
  static int mRenderWidth;
  static int mRenderHeight;
  static float mRefreshRate;
  static bool mLockedFramerate;
  static bool m2DCollisionOnly;
  static int mPacketSize;

public:
  SystemProperties();
  virtual ~SystemProperties();

  static HashString const &GetAssetsDirectory();
  static HashString const &GetDefaultVertexShaderName();
  static HashString const &GetDefaultFragmentShaderName();
  static HashString const &GetFramebufferVertexShaderName();
  static HashString const &GetFramebufferFragmentShaderName();
  static HashString const &GetMagFilter();
  static HashString const &GetMinFilter();
  static HashString const &GetWrapModeS();
  static HashString const &GetWrapModeT();
  
  static bool const GetFullScreen();
  static HashString const &GetGameTitle();
  static int const GetScreenWidth();
  static int const GetScreenHeight();
  static int const GetRenderWidth();
  static int const GetRenderHeight();
  static float const GetRefreshRate();
  static bool const GetLockedFramerate();
  static bool const Get2DCollisionOnly();
  static int const GetPacketSize();
  
  static void Serialize();
  static void Deserialize();
  static void SerializeLUA();
};

#endif
