#include "openGL.h"

#include "shader.h"
#include "texture.h"
#include "window.h"
#include <utility>

class OpenGL::PrivateData
{
  public:
    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };
    std::shared_ptr< Window > window;
    std::unique_ptr< Shader > shaderProgram;
    std::unique_ptr< Texture > texture;
    unsigned int size{ 0 };
};

OpenGL::OpenGL( const std::shared_ptr< Window >& window )
  : m_data( new PrivateData() )
{
  m_data->window = window;
}

OpenGL::~OpenGL()
{
  glDeleteVertexArrays( 1, &m_data->VAO );
  glDeleteBuffers( 1, &m_data->VBO );
  glDeleteBuffers( 1, &m_data->EBO );
}

GLFWwindow* OpenGL::window()
{
  return m_data->window->handle();
}

void OpenGL::setBackgroundColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
  glClearColor( red, green, blue, alpha );
  glClear( GL_COLOR_BUFFER_BIT );
}

void OpenGL::prepareVertices( float* vertices, long long size )
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

void OpenGL::prepareIndices( unsigned int* indices, long long int size )
{
  assert( m_data->window );
  glGenBuffers( 1, &m_data->EBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_data->EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
  m_data->size = size;
}

void OpenGL::pollEvents()
{
  glfwPollEvents();
}

void OpenGL::drawVertices( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const
{
  assert( m_data->window );
  m_data->texture->use();
  m_data->shaderProgram->use( model, view, projection );
  glBindVertexArray( m_data->VAO );
  glEnable( GL_PRIMITIVE_RESTART );
  glPrimitiveRestartIndex( 0xFFFF );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glDrawElements( GL_TRIANGLE_STRIP, ( GLint ) m_data->size, GL_UNSIGNED_INT, nullptr );
}

void OpenGL::createShader( const char* vertexPath, const char* fragmentPath )
{
  assert( m_data->window );
  m_data->shaderProgram = std::make_unique< Shader >( vertexPath, fragmentPath );
}

void OpenGL::createTexture( const char* texturePath )
{
  assert( m_data->window );
  m_data->texture = std::make_unique< Texture >( texturePath );
}
void OpenGL::swapBuffers()
{
  assert( m_data->window );
  glfwSwapBuffers( m_data->window->handle() );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
