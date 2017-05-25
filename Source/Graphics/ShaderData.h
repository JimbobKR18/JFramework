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
  unsigned mVertexShaderID;
  unsigned mFragmentShaderID;
  HashString mVertexContents;
  HashString mFragmentContents;

  ShaderData() : mProgramID(0), mVertexShaderID(0), mFragmentShaderID(0),
                 mVertexContents(), mFragmentContents() {}
  ShaderData(ShaderData const &aRhs) : mProgramID(aRhs.mProgramID), mVertexShaderID(aRhs.mVertexShaderID), 
    mFragmentShaderID(aRhs.mFragmentShaderID), mVertexContents(aRhs.mVertexContents), mFragmentContents(aRhs.mFragmentContents) {}
  ShaderData(unsigned aProgramID, unsigned aVertexShaderID, unsigned aFragmentShaderID, 
             HashString const &aVertexContents, HashString const &aFragmentContents) :
             mProgramID(aProgramID), mVertexShaderID(aVertexShaderID), mFragmentShaderID(aFragmentShaderID), 
             mVertexContents(aVertexContents), mFragmentContents(aFragmentContents) {}
  void operator=(ShaderData const &aRhs)
  {
    mProgramID = aRhs.mProgramID;
    mVertexShaderID = aRhs.mVertexShaderID;
    mFragmentShaderID = aRhs.mFragmentShaderID;
    mVertexContents = aRhs.mVertexContents;
    mFragmentContents = aRhs.mFragmentContents;
  }
};

#endif /* TEXTUREDATA_H_ */