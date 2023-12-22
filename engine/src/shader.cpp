#include "shader.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
  std::basic_string< char, std::char_traits< char >, std::allocator< char > > readShaderCode( const char* shaderPath )
  {
    std::ifstream shaderFile;
    std::stringstream shaderStream;
    shaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
      shaderFile.open( shaderPath );
      shaderStream << shaderFile.rdbuf();
      shaderFile.close();
    }
    catch( std::ifstream::failure& e )
    {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    return shaderStream.str();
  }
}

class Shader::PrivateData
{
  public:
    unsigned int shaderProgram = 0;
};

Shader::Shader( const char* vertexPath, const char* fragmentPath )
  : m_data{ new PrivateData() }
{
  createShaderProgram( createAndCompileShader( readShaderCode( vertexPath ).c_str(), GL_VERTEX_SHADER ),
    createAndCompileShader( readShaderCode( fragmentPath ).c_str(), GL_FRAGMENT_SHADER ) );
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

void Shader::use()
{
  assert( m_data->shaderProgram != 0 );
  glUseProgram( m_data->shaderProgram );
}

int Shader::getUniformLocation( const char* uniformValue )
{
  return glGetUniformLocation( m_data->shaderProgram, uniformValue );
}

void Shader::setBool( const std::string& name, bool value ) const
{
  glUniform1i( glGetUniformLocation( m_data->shaderProgram, name.c_str() ), ( int ) value );
}

void Shader::setInt( const std::string& name, int value ) const
{
  glUniform1i( glGetUniformLocation( m_data->shaderProgram, name.c_str() ), value );
}

void Shader::setFloat( const std::string& name, float value ) const
{
  glUniform1f( glGetUniformLocation( m_data->shaderProgram, name.c_str() ), value );
}
