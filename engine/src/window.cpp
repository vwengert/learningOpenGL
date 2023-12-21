#include "window.h"

#include <iostream>

namespace
{
  void frameBufferSizeCallback( GLFWwindow*, int width, int height )
  {
    glViewport( 0, 0, width, height );
  }
}

class Window::PrivateData
{
  public:
    GLFWwindow* window{ nullptr };
};

Window::Window( int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share )
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

Window::~Window()
{
  glfwDestroyWindow( m_data->window );
}

GLFWwindow* Window::window()
{
  return m_data->window;
}
