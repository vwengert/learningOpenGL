#include "shader.h"

#include <cassert>
#include <iostream>

class Shader::PrivateData
{
  public:
    unsigned int shaderProgram = 0;
};

Shader::Shader( const char* vertexShaderSource, const char* fragmentShaderSource )
  : m_data{ new PrivateData() }
{
  createShaderProgram( createAndCompileShader( vertexShaderSource, GL_VERTEX_SHADER ),
    createAndCompileShader( fragmentShaderSource, GL_FRAGMENT_SHADER ) );
}

Shader::~Shader()
{
  if( m_data->shaderProgram )
  {
    glDeleteProgram( m_data->shaderProgram );
  }
}

unsigned int Shader::createAndCompileShader( const char* shaderGLSL, unsigned int shaderType )
{
  unsigned int vertexShader;
  vertexShader = glCreateShader( shaderType );
  glShaderSource( vertexShader, 1, &shaderGLSL, nullptr );
  glCompileShader( vertexShader );

  int success;
  char infoLog[ 512 ];
  glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
  if( !success )
  {
    glGetShaderInfoLog( vertexShader, 512, nullptr, infoLog );
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    throw;
  }
  return vertexShader;
}

void Shader::createShaderProgram( unsigned int vertexShader, unsigned int fragmentShader )
{
  m_data->shaderProgram = glCreateProgram();
  glAttachShader( m_data->shaderProgram, vertexShader );
  glAttachShader( m_data->shaderProgram, fragmentShader );

  glLinkProgram( m_data->shaderProgram );

  int success;
  char infoLog[ 512 ];
  glGetProgramiv( m_data->shaderProgram, GL_LINK_STATUS, &success );
  if( !success )
  {
    glGetProgramInfoLog( m_data->shaderProgram, 512, nullptr, infoLog );
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    throw;
  }

  glDeleteShader( vertexShader );
  glDeleteShader( fragmentShader );
}

void Shader::useShaderProgram()
{
  assert( m_data->shaderProgram != 0 );
  glUseProgram( m_data->shaderProgram );
}
