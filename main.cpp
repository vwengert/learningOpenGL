#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <openGL.h>

auto main() -> int
{
  // TODO: change 800 and 600 with real values
  constexpr int width = 800;
  constexpr int height = 600;
  OpenGL engine( width, height, "modern C++" );
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

  while( !glfwWindowShouldClose( engine.window() ) )
  {
    engine.processInput();
    OpenGL::setBackgroundColor( 0.2f, 0.3f, 0.3f, 1.0f );

    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 view = glm::mat4( 1.0f );
    glm::mat4 projection = glm::mat4( 1.0f );

    model = glm::rotate( model, glm::radians( -55.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
    view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );
    projection = glm::perspective( glm::radians( 45.0f ), ( float ) width / ( float ) height, 0.1f, 100.0f );

    engine.drawVertices( model, view, projection );
    OpenGL::pollEvents();
  }

  return 0;
}