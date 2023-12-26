#include "openGL.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <valarray>

class OpenGL::PrivateData
{
  public:
    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };
    std::unique_ptr< Window > window;
    std::unique_ptr< Shader > shaderProgram;
    std::unique_ptr< Texture > texture;
};

OpenGL::OpenGL( int width, int height, const char* title )
  : m_data( new PrivateData() )
{
  m_data->window = std::make_unique< Window >( width, height, title, nullptr, nullptr );
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
  glGenVertexArrays( 1, &m_data->VAO );
  glGenBuffers( 1, &m_data->VBO );
  glGenBuffers( 1, &m_data->EBO );

  glBindVertexArray( m_data->VAO );

  glBindBuffer( GL_ARRAY_BUFFER, m_data->VBO );
  glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_data->EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) 0 );
  glEnableVertexAttribArray( 0 );

  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
  glEnableVertexAttribArray( 1 );

  glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 6 * sizeof( float ) ) );
  glEnableVertexAttribArray( 2 );
}

void OpenGL::prepareIndices( unsigned int* indices, long long int size )
{
  glGenBuffers( 1, &m_data->EBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_data->EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
}

void OpenGL::processInput()
{
  if( glfwGetKey( m_data->window->handle(), GLFW_KEY_ESCAPE ) == GLFW_PRESS )
  {
    glfwSetWindowShouldClose( m_data->window->handle(), true );
  }
}

void OpenGL::pollEvents()
{
  glfwPollEvents();
}

void OpenGL::drawVertices( glm::mat4 transform ) const
{
  m_data->texture->use();
  m_data->shaderProgram->use( transform );
  glBindVertexArray( m_data->VAO );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

  glfwSwapBuffers( m_data->window->handle() );
}

void OpenGL::createShader( const char* vertexPath, const char* fragmentPath )
{
  m_data->shaderProgram = std::make_unique< Shader >( vertexPath, fragmentPath );
}

void OpenGL::createTexture( const char* texturePath )
{
  m_data->texture = std::make_unique< Texture >( texturePath );
}
