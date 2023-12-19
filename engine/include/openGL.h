#ifndef LEARNOPENGL_OPENGL_H
#define LEARNOPENGL_OPENGL_H

#include <vector>
#include <glad/glad.h>
#include <glfw/glfw3.h>

class GLFWwindow;

class OpenGL {
public:
    OpenGL(int width, int height, const char *title);

    ~OpenGL();

    static unsigned int createAndCompileShader(const char *shaderGLSL, unsigned int shaderType);

    void createShaderProgram(const std::vector<unsigned int> &shaders);

    static void setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    static void pollEvents();

    void prepareVertices(float *vertices, long long size);

    void prepareIndices(unsigned int *indices, long long size);

    void drawVertices() const;

    void processInput();

    GLFWwindow *window();

private:
    GLFWwindow *m_window;
    unsigned int m_VAO{0};
    unsigned int m_VBO{0};
    unsigned int m_EBO{0};
    unsigned int m_shaderProgram{0};
};


#endif //LEARNOPENGL_OPENGL_H
