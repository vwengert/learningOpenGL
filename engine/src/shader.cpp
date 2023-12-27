#include "shader.h"

#include <cassert>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

namespace
{
  std::string readShaderCode( const char* shaderPath )
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
      std::cout << shaderPath << '\n';
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    return shaderStream.str();
  }

  unsigned int createAndCompileShader( const char* shaderGLSL, unsigned int shaderType )
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

  unsigned int createShaderProgram( unsigned int vertexShader, unsigned int fragmentShader )
  {
    auto ID = glCreateProgram();
    glAttachShader( ID, vertexShader );
    glAttachShader( ID, fragmentShader );

    glLinkProgram( ID );

    int success;
    char infoLog[ 512 ];
    glGetProgramiv( ID, GL_LINK_STATUS, &success );
    if( !success )
    {
      glGetProgramInfoLog( ID, 512, nullptr, infoLog );
      std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
      throw;
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return ID;
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
  m_data->shaderProgram =
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

void Shader::use( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const
{
  glUniformMatrix4fv( getUniformLocation( "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
  glUniformMatrix4fv( getUniformLocation( "view" ), 1, GL_FALSE, &view[ 0 ][ 0 ] );
  glUniformMatrix4fv( getUniformLocation( "projection" ), 1, GL_FALSE, &projection[ 0 ][ 0 ] );
  glUseProgram( m_data->shaderProgram );
}

[[maybe_unused]] int Shader::getUniformLocation( const char* uniformValue ) const
{
  return glGetUniformLocation( m_data->shaderProgram, uniformValue );
}

[[maybe_unused]] void Shader::setBool( const std::string& name, bool value ) const
{
  glUniform1i( glGetUniformLocation( m_data->shaderProgram, name.c_str() ), ( int ) value );
}

[[maybe_unused]] void Shader::setInt( const std::string& name, int value ) const
{
  glUniform1i( glGetUniformLocation( m_data->shaderProgram, name.c_str() ), value );
}

[[maybe_unused]] void Shader::setFloat( const std::string& name, float value ) const
{
  glUniform1f( glGetUniformLocation( m_data->shaderProgram, name.c_str() ), value );
}
