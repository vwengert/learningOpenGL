#include "openGL.h"
#include <iostream>

namespace {
    void frameBufferSizeCallback(GLFWwindow *, int width, int height) {
        glViewport(0, 0, width, height);
    }
}

OpenGL::OpenGL(int width, int height, const char *title)
        : m_window{nullptr} {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        throw;
    }

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw;
    }

    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
}

OpenGL::~OpenGL() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_shaderProgram);
    glfwTerminate();
}

GLFWwindow *OpenGL::window() {
    return m_window;
}

void OpenGL::setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int OpenGL::createAndCompileShader(const char *shaderGLSL, unsigned int shaderType) {
    unsigned int vertexShader;
    vertexShader = glCreateShader(shaderType);
    glShaderSource(vertexShader, 1, &shaderGLSL, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw;
    }
    return vertexShader;
}

void OpenGL::createShaderProgram(const std::vector<unsigned int> &shaders) {
    m_shaderProgram = glCreateProgram();
    for (auto shader: shaders) {
        glAttachShader(m_shaderProgram, shader);
    }
    glLinkProgram(m_shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        throw;
    }

    for (auto shader: shaders) {
        glDeleteShader(shader);
    }
}

void OpenGL::prepareVertices(float *vertices, long long size) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
}

void OpenGL::prepareIndices(unsigned int *indices, long long int size) {
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void OpenGL::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void OpenGL::pollEvents() {
    glfwPollEvents();
}

void OpenGL::drawVertices() const {
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_VAO);
    static int x = 0;
    if (x > 40) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (x > 80) {
        x = 0;
    }
    ++x;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glfwSwapBuffers(m_window);
}