#ifndef LEARNOPENGL_OPENGL_H
#define LEARNOPENGL_OPENGL_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class GLFWwindow;

class OpenGL
{
  public:
    OpenGL( int width, int height, const char* title );
    ~OpenGL();

    void createShader( const char* vertexShaderSource, const char* fragmentShaderSource );
    void createTexture( const char* texturePath );
    static void setBackgroundColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
    static void pollEvents();
    void prepareVertices( float* vertices, long long size );
    [[maybe_unused]] void prepareIndices( unsigned int* indices, long long size );
    void drawVertices( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const;
    void processInput();
    void swapBuffers();
    GLFWwindow* window();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif // LEARNOPENGL_OPENGL_H
