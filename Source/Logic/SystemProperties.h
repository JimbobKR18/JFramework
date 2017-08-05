/*
 * SystemProperties.h
 *
 *  Created on: May 17, 2016
 *      Author: jimbob
 */

#ifndef __JFramework_SystemProperties_h_
#define __JFramework_SystemProperties_h_

#include "Common.h"
#include "TextParser.h"

class SystemProperties
{
private:
  static HashString mAssetsDirectory;

public:
  SystemProperties();
  virtual ~SystemProperties();

  static HashString const &GetAssetsDirectory();
  static void Deserialize();
  static void SerializeLUA();
};

#endif
