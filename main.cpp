#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <openGL.h>

auto main() -> int {
    OpenGL engine(800, 600, "modern C++");
    engine.createShaderProgram({OpenGL::createAndCompileShader("#version 330 core\n"
                                                               "layout (location = 0) in vec3 aPos;\n"
                                                               "void main()\n"
                                                               "{\n"
                                                               " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                                               "}\n",
                                                               GL_VERTEX_SHADER),
                                OpenGL::createAndCompileShader("#version 330 core\n"
                                                               "out vec4 FragColor;\n"
                                                               "void main()\n"
                                                               "{\n"
                                                               " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                                               "}\n",
                                                               GL_FRAGMENT_SHADER)});
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    engine.prepareVertices(vertices, sizeof vertices);
    engine.prepareIndices(indices, sizeof indices);

    while (!glfwWindowShouldClose(engine.window())) {
        engine.processInput();
        OpenGL::setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
        engine.drawVertices();
        OpenGL::pollEvents();
    }

    return 0;
}