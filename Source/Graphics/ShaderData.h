/*
 * ShaderData.h
 *
 *  Created on: Jan 30, 2015
 *      Author: jimbob
 */

#ifndef SHADERDATA_H_
#define SHADERDATA_H_

// TODO may be worth to split up vertex and fragment shaders.
// Use multiple programs somehow.
struct ShaderData
{
  unsigned mProgramID;
  unsigned mShaderID;
  HashString mContents;

  ShaderData() : mProgramID(0), mShaderID(0), mContents() {}
  ShaderData(unsigned aProgramID, unsigned aShaderID, HashString const &aContents) :
    mProgramID(aProgramID), mShaderID(aShaderID), mContents(aContents) {}
  void operator=(ShaderData const &aRhs)
  {
    mProgramID = aRhs.mProgramID;
    mShaderID = aRhs.mShaderID;
    mContents = aRhs.mContents;
  }
};

#endif /* TEXTUREDATA_H_ */