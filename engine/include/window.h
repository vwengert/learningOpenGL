#ifndef LEARNOPENGL_WINDOW_H
#define LEARNOPENGL_WINDOW_H

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <memory>

class Window
{
  public:
    explicit Window( int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share );
    ~Window();

    GLFWwindow* handle();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif // LEARNOPENGL_WINDOW_H
