#include "ShaderLoader.h"

#if !defined(_WIN32) && !defined(__APPLE__)
#include <SDL2/SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2_image/SDL_image.h>
#else
#include "Windows.h"
#include <GL\glew.h>
#include "SDL_image.h"
#undef LoadImage
#undef SendMessage
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#endif

ShaderLoader::ShaderLoader()
{
}

ShaderLoader::~ShaderLoader()
{
}

std::vector<HashString> ShaderLoader::LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename)
{
  std::vector<HashString> ret;
  
  std::ifstream vertexFile(Common::RelativePath("Shaders", aVertexShaderFilename.ToCharArray()).c_str());
  std::ifstream fragmentFile(Common::RelativePath("Shaders", aFragmentShaderFilename.ToCharArray()).c_str());
  if(vertexFile.is_open() && fragmentFile.is_open())
  {
    GLuint program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint didCompile = 0;
    GLint isLinked = 0;
    
    HashString vertexFileContents = std::string((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
    const char* vertexContents = vertexFileContents.ToCharArray();
    
    // Compile
    glShaderSource(vertexShader, 1, &vertexContents, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &didCompile);
    if(didCompile == GL_FALSE)
    {
      DebugLogPrint("VERTEX SHADER %s READ:\n%s\n", aVertexShaderFilename.ToCharArray(), vertexContents);
      
      GLint maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<char> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
      
      DebugLogPrint("GLERROR %s: %s\n", aVertexShaderFilename.ToCharArray(), &infoLog[0]);
	
      //We don't need the shader anymore.
      glDeleteShader(vertexShader);
    }
    
    HashString fragmentFileContents = std::string((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());
    const char* fragmentContents = fragmentFileContents.ToCharArray();
    glShaderSource(fragmentShader, 1, &fragmentContents, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &didCompile);
    if(didCompile == GL_FALSE)
    {
      DebugLogPrint("FRAGMENT SHADER %s READ:\n%s\n", aFragmentShaderFilename.ToCharArray(), fragmentContents);
      
      GLint maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<char> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
      
      DebugLogPrint("GLERROR %s: %s\n", aFragmentShaderFilename.ToCharArray(), &infoLog[0]);
	
      //We don't need the shader anymore.
      glDeleteShader(fragmentShader);
    }
    
    // Linking
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      //The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
      //We don't need the program anymore.
      glDeleteProgram(program);
      //Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      DebugLogPrint("GL LINK ERROR: %s\n", &infoLog[0]);
    }
    
    ret.push_back(Common::IntToString(program));
    ret.push_back(Common::IntToString(vertexShader));
    ret.push_back(Common::IntToString(fragmentShader));
    ret.push_back(vertexContents);
    ret.push_back(fragmentContents);
  }
  else
  {
    DebugLogPrint("No shader with name %s or %s found\n", aVertexShaderFilename.ToCharArray(), aFragmentShaderFilename.ToCharArray());
    assert(!"Shader file not found.");
  }
  
  return ret;
}
