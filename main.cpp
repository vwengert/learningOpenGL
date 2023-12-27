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
     0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
     0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
    -0.5f,  0.5f, 0.5f,     1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f,     0.0f, 0.0f
};

  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3,
      0, 3, 7,
      0, 4, 7,
      1, 2, 6,
      1, 5, 6,
      4, 5, 6,
      4, 7, 6,
      0, 4, 5,
      0, 1, 5,
      3, 7, 6,
      3, 2, 6,
  };

  glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f, 0.0f, 0.0f),
      glm::vec3( 2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f, 2.0f, -2.5f),
      glm::vec3( 1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)
  };
  // clang-format on

  engine.prepareVertices( vertices, sizeof vertices );
  engine.prepareIndices( indices, sizeof indices );

  while( !glfwWindowShouldClose( engine.window() ) )
  {
    engine.processInput();
    OpenGL::setBackgroundColor( 0.2f, 0.3f, 0.3f, 1.0f );

    glm::mat4 view = glm::mat4( 1.0f );
    glm::mat4 projection = glm::mat4( 1.0f );

    view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );
    projection = glm::perspective( glm::radians( 45.0f ), ( float ) width / ( float ) height, 0.1f, 100.0f );

    for( unsigned int i = 0; i < 10; ++i )
    {
      glm::mat4 model = glm::mat4( 1.0f );
      model = glm::translate( model, cubePositions[ i ] );
      float angle = 11.1f * ( float ) ( i + 1 );
      model = glm::rotate( model, glm::radians( angle * ( float ) glfwGetTime() ), glm::vec3( 1.0f, 0.3f, 0.5f ) );
      engine.drawVertices( model, view, projection );
    }

    engine.swapBuffers();
    OpenGL::pollEvents();
  }

  return 0;
}