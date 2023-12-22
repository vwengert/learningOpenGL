#include <openGL.h>

auto main() -> int
{
  OpenGL engine( 800, 600, "modern C++" );
  engine.createShader( "#version 330 core\n"
                       "layout (location = 0) in vec3 aPos;\n"
                       "layout (location = 1) in vec3 aColor;\n"
                       "out vec3 ourColor;\n"
                       "void main()\n"
                       "{\n"
                       " gl_Position = vec4(aPos, 1.0);\n"
                       "ourColor = aColor;\n"
                       "}\n",
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(ourColor, 1.0f);\n"
    "}\n" );
  float vertices[] = { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f,
    0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f };

  unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

  engine.prepareVertices( vertices, sizeof vertices );
  engine.prepareIndices( indices, sizeof indices );

  while( !glfwWindowShouldClose( engine.window() ) )
  {
    engine.processInput();
    OpenGL::setBackgroundColor( 0.2f, 0.3f, 0.3f, 1.0f );
    engine.drawVertices();
    OpenGL::pollEvents();
  }

  return 0;
}