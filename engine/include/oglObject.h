#ifndef LEARNOPENGL_OPENGL_H
#define LEARNOPENGL_OPENGL_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <memory>

class GLFWwindow;
class Window;
class Shader;
class Texture;

class OglObject
{
  public:
    explicit OglObject( const std::shared_ptr< Window >& window );
    ~OglObject();

    void setShader( std::shared_ptr< Shader > shader );
    void setTexture( std::shared_ptr< Texture > texture );
    void prepareVertices( float* vertices, long long size );
    [[maybe_unused]] void prepareIndices( unsigned int* indices, long long size );
    void drawVertices( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const;
    GLFWwindow* window();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif // LEARNOPENGL_OPENGL_H
