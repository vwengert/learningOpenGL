#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <openGL.h>
#include <window.h>

void processInput( GLFWwindow* window );
void mouseCallback( GLFWwindow* window, double xPos, double yPos );
void scrollCallback( GLFWwindow* window, double xOffset, double yOffset );

glm::vec3 cameraPos = glm::vec3( 0.0f, 0.0f, -35.0f );
glm::vec3 cameraFront = glm::vec3( 0.0f, 0.0f, 1.0f );
glm::vec3 cameraUp = glm::vec3( 0.0f, 1.0f, 0.0f );
float yaw = 90.0f;
float pitch = 0.0f;
float zoom = 35.0f;
bool firstMouse = true;

auto main() -> int
{
  // TODO: change 800 and 600 with real values
  constexpr int width = 800;
  constexpr int height = 600;
  const char* title = "modern C++";
  const auto window = std::make_shared< Window >( width, height, title, nullptr, nullptr );
  OpenGL engine( window );
  engine.createShader( "resource/cubes.vert", "resource/cubes.frag" );
  engine.createTexture( "resource/pfote.jpg" );

  // clang-format off
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,     1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
    0.5f, 0.5f, 0.5f,     1.0f, 1.0f, 0.0f,     0.0f, 1.0f
};

  unsigned int indices[] = {
      0, 1, 2, 3, 6, 7, 4, 5,
      0xFFFF,
      2, 6, 0, 4, 1, 5, 3, 7
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
  glfwSetInputMode( engine.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
  glfwSetCursorPosCallback( engine.window(), mouseCallback );
  glfwSetScrollCallback( engine.window(), scrollCallback );

  while( !glfwWindowShouldClose( engine.window() ) )
  {
    processInput( engine.window() );
    OpenGL::setBackgroundColor( 0.2f, 0.3f, 0.3f, 1.0f );

    glm::mat4 view = glm::mat4( 1.0f );
    glm::mat4 projection = glm::mat4( 1.0f );

    glm::vec3 direction;
    direction.x = ( float ) ( cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ) );
    direction.y = ( float ) ( sin( glm::radians( pitch ) ) );
    direction.z = ( float ) ( sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ) );
    cameraFront = glm::normalize( direction );

    view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp );
    projection = glm::perspective( glm::radians( zoom ), ( float ) width / ( float ) height, 0.1f, 100.0f );

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

void processInput( GLFWwindow* window )
{
  static float deltaTime = 0.0f;
  static float lastFrame = 0.0f;
  auto currentFrame = ( float ) glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
  {
    glfwSetWindowShouldClose( window, true );
  }

  const float cameraSpeed = 2.5f;
  if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
  {
    cameraPos += cameraSpeed * deltaTime * cameraFront;
  }
  if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
  {
    cameraPos -= cameraSpeed * deltaTime * cameraFront;
  }
  if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
  {
    cameraPos -= glm::normalize( glm::cross( cameraFront, cameraUp ) ) * cameraSpeed * deltaTime;
  }
  if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
  {
    cameraPos += glm::normalize( glm::cross( cameraFront, cameraUp ) ) * cameraSpeed * deltaTime;
  }
}

void mouseCallback( GLFWwindow*, double xPos, double yPos )
{
  static float lastX, lastY;
  if( firstMouse )
  {
    lastX = ( float ) xPos;
    lastY = ( float ) yPos;
    firstMouse = false;
    return;
  }
  auto xOffset = ( float ) xPos - lastX;
  auto yOffset = ( float ) yPos - lastY;
  lastX = ( float ) xPos;
  lastY = ( float ) yPos;
  const float sensitivity = 0.1f;
  xOffset *= sensitivity;
  yOffset *= sensitivity;
  yaw += xOffset;
  pitch += yOffset;
  if( pitch > 89.0f )
  {
    pitch = 89.0f;
  }
  if( pitch < -89.0f )
  {
    pitch = -89.0f;
  }
}

void scrollCallback( GLFWwindow*, double, double yOffset )
{
  zoom -= ( float ) yOffset;
  if( zoom < 1.0f )
  {
    zoom = 1.0f;
  }
  if( zoom > 45.0f )
  {
    zoom = 45.0f;
  }
}
