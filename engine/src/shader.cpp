#include "shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

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
      std::cerr << shaderPath << '\n';
      std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
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
      std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
      throw;
    }
    return vertexShader;
  }

  unsigned int createProgram( const std::vector< unsigned int >& shaders )
  {
    auto ID = glCreateProgram();
    for( auto shader : shaders )
    {
      glAttachShader( ID, shader );
    }

    glLinkProgram( ID );

    int success;
    char infoLog[ 512 ];
    glGetProgramiv( ID, GL_LINK_STATUS, &success );
    if( !success )
    {
      glGetProgramInfoLog( ID, 512, nullptr, infoLog );
      std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
      throw;
    }

    for( auto shader : shaders )
    {
      glDeleteShader( shader );
    }

    return ID;
  }
}

class Shader::PrivateData
{
  public:
    unsigned int shaderProgram = 0;
    std::map< unsigned int, std::string > shaders;
};

Shader::Shader()
  : m_data{ new PrivateData() }
{
}

Shader::Shader( const char* vertexPath, const char* fragmentPath )
  : Shader()
{
  addShader( GL_VERTEX_SHADER, vertexPath );
  addShader( GL_FRAGMENT_SHADER, fragmentPath );
  createShaderProgram();
}

Shader::~Shader()
{
  if( m_data->shaderProgram )
  {
    glDeleteProgram( m_data->shaderProgram );
  }
}

void Shader::addShader( unsigned int type, const char* shaderPath ) const
{
  m_data->shaders[ type ] = readShaderCode( shaderPath );
}

void Shader::createShaderProgram() const
{
  if( m_data->shaderProgram > 0 )
  {
    std::cerr << "Program already compiled" << std::endl;
    return;
  }
  if( m_data->shaders.find( GL_VERTEX_SHADER ) == m_data->shaders.end() )
  {
    std::cerr << "Minimum Vertex Shader is missing" << std::endl;
  }

  std::vector< unsigned int > shaders;

  for( const auto& shader : m_data->shaders )
  {
    shaders.push_back( createAndCompileShader( shader.second.c_str(), shader.first ) );
  }

  m_data->shaderProgram = createProgram( shaders );
}

void Shader::use( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const
{
  if( m_data->shaderProgram == 0 )
  {
    createShaderProgram();
  }
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
