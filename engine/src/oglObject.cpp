#include "oglObject.h"

#include "shader.h"
#include "texture.h"
#include "window.h"

class OglObject::PrivateData
{
  public:
    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };
    std::shared_ptr< Window > window{ nullptr };
    std::shared_ptr< Shader > shaderProgram{ nullptr };
    std::shared_ptr< Texture > texture{ nullptr };
    unsigned int size{ 0 };
};

OglObject::OglObject( const std::shared_ptr< Window >& window )
  : m_data( new PrivateData() )
{
  m_data->window = window;
}

OglObject::~OglObject()
{
  glDeleteVertexArrays( 1, &m_data->VAO );
  glDeleteBuffers( 1, &m_data->VBO );
  glDeleteBuffers( 1, &m_data->EBO );
}

GLFWwindow* OglObject::window()
{
  return m_data->window->handle();
}

void OglObject::prepareVertices( float* vertices, long long size )
{
  assert( m_data->window );
  glGenVertexArrays( 1, &m_data->VAO );
  glGenBuffers( 1, &m_data->VBO );
  glGenBuffers( 1, &m_data->EBO );

  glBindVertexArray( m_data->VAO );

  glBindBuffer( GL_ARRAY_BUFFER, m_data->VBO );
  glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_data->EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) nullptr );
  glEnableVertexAttribArray( 0 );

  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
  glEnableVertexAttribArray( 1 );

  glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 6 * sizeof( float ) ) );
  glEnableVertexAttribArray( 2 );

  glEnable( GL_DEPTH_TEST );
}

void OglObject::prepareIndices( unsigned int* indices, long long int size )
{
  assert( m_data->window );
  glGenBuffers( 1, &m_data->EBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_data->EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
  m_data->size = size;
}

void OglObject::drawVertices( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const
{
  assert( m_data->window );
  assert( m_data->shaderProgram );
  assert( m_data->texture );
  m_data->texture->use();
  m_data->shaderProgram->use( model, view, projection );
  glBindVertexArray( m_data->VAO );
  glEnable( GL_PRIMITIVE_RESTART );
  glPrimitiveRestartIndex( 0xFFFF );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glDrawElements( GL_TRIANGLE_STRIP, ( GLint ) m_data->size, GL_UNSIGNED_INT, nullptr );
}

void OglObject::setShader( std::shared_ptr< Shader > shader )
{
  m_data->shaderProgram = shader;
}

void OglObject::setTexture( std::shared_ptr< Texture > texture )
{
  m_data->texture = texture;
}
