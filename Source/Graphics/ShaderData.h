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
  HashString mVertexFileName;
  HashString mFragmentFileName;

  ShaderData() : mProgramID(0), mVertexShaderID(0), mFragmentShaderID(0),
                 mVertexContents(), mFragmentContents(), mVertexFileName(), mFragmentFileName() {}
  ShaderData(ShaderData const &aRhs) : mProgramID(aRhs.mProgramID), mVertexShaderID(aRhs.mVertexShaderID), 
    mFragmentShaderID(aRhs.mFragmentShaderID), mVertexContents(aRhs.mVertexContents), mFragmentContents(aRhs.mFragmentContents),
    mVertexFileName(aRhs.mVertexFileName), mFragmentFileName(aRhs.mFragmentFileName) {}
  ShaderData(unsigned aProgramID, unsigned aVertexShaderID, unsigned aFragmentShaderID, 
             HashString const &aVertexContents, HashString const &aFragmentContents,
             HashString const &aVertexFileName, HashString const &aFragmentFileName) :
             mProgramID(aProgramID), mVertexShaderID(aVertexShaderID), mFragmentShaderID(aFragmentShaderID), 
             mVertexContents(aVertexContents), mFragmentContents(aFragmentContents), mVertexFileName(aVertexFileName),
             mFragmentFileName(aFragmentFileName) {}
  void operator=(ShaderData const &aRhs)
  {
    mProgramID = aRhs.mProgramID;
    mVertexShaderID = aRhs.mVertexShaderID;
    mFragmentShaderID = aRhs.mFragmentShaderID;
    mVertexContents = aRhs.mVertexContents;
    mFragmentContents = aRhs.mFragmentContents;
    mVertexFileName = aRhs.mVertexFileName;
    mFragmentFileName = aRhs.mFragmentFileName;
  }
};

#endif /* TEXTUREDATA_H_ */