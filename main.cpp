#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <openGL.h>

auto main() -> int
{
  OpenGL engine( 800, 600, "modern C++" );
  engine.createShader( "resource/vertex.shader", "resource/fragment.shader" );
  engine.createTexture( "resource/pfote.jpg" );

  // clang-format off
  float vertices[] = {
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,     0.0f, 0.0f
};

  unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };
  // clang-format on

  engine.prepareVertices( vertices, sizeof vertices );
  engine.prepareIndices( indices, sizeof indices );

  float radian = 0.0f;
  while( !glfwWindowShouldClose( engine.window() ) )
  {
    engine.processInput();
    OpenGL::setBackgroundColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glm::mat4 transform = glm::mat4( 1.0f );
    transform = glm::translate( transform, glm::vec3( 0.0f, 0.0f, 0.0f ) );
    transform = glm::rotate( transform, glm::radians( radian ), glm::vec3( 1.0, 0.0, 0.0 ) );
    transform = glm::scale( transform, glm::vec3( 0.9, 0.9, 0.0 ) );
    radian += 1.0f;
    if( radian > 359.0f )
    {
      radian = 0.0f;
    }
    engine.drawVertices( transform );
    OpenGL::pollEvents();
  }

  return 0;
}