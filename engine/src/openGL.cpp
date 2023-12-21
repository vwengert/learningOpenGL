#include "openGL.h"
#include "shader.h"
#include <iostream>

namespace
{
  void frameBufferSizeCallback( GLFWwindow*, int width, int height )
  {
    glViewport( 0, 0, width, height );
  }
}

class OpenGL::PrivateData
{
  public:
    GLFWwindow* window{ nullptr };
    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
    unsigned int EBO{ 0 };
    std::unique_ptr< Shader > shaderProgram;
};

OpenGL::OpenGL( int width, int height, const char* title )
  : m_data( new PrivateData() )
{
  glfwInit();
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  m_data->window = glfwCreateWindow( width, height, title, nullptr, nullptr );
  if( m_data->window == nullptr )
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    throw;
  }

  glfwMakeContextCurrent( m_data->window );
  if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    throw;
  }

  glfwSetFramebufferSizeCallback( m_data->window, frameBufferSizeCallback );
}

OpenGL::~OpenGL()
{
  glDeleteVertexArrays( 1, &m_data->VAO );
  glDeleteBuffers( 1, &m_data->VBO );
  glfwTerminate();
}

GLFWwindow* OpenGL::window()
{
  return m_data->window;
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

  glBindVertexArray( m_data->VAO );

  glBindBuffer( GL_ARRAY_BUFFER, m_data->VBO );
  glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* ) 0 );
  glEnableVertexAttribArray( 0 );
}

void OpenGL::prepareIndices( unsigned int* indices, long long int size )
{
  glGenBuffers( 1, &m_data->EBO );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_data->EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
}

void OpenGL::processInput()
{
  if( glfwGetKey( m_data->window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
  {
    glfwSetWindowShouldClose( m_data->window, true );
  }
}

void OpenGL::pollEvents()
{
  glfwPollEvents();
}

void OpenGL::drawVertices() const
{
  m_data->shaderProgram->useShaderProgram();
  glBindVertexArray( m_data->VAO );
  static int x = 0;
  if( x > 40 )
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }
  else
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
  if( x > 80 )
  {
    x = 0;
  }
  ++x;
  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 );
  glfwSwapBuffers( m_data->window );
}

void OpenGL::createShader( const char* vertexShaderSource, const char* fragmentShaderSource )
{
  m_data->shaderProgram = std::make_unique< Shader >( vertexShaderSource, fragmentShaderSource );
}
