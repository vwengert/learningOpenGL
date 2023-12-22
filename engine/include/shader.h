#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>
#include <memory>

class Shader
{
  public:
    explicit Shader( const char* vertexShaderSource, const char* fragmentShaderSource );
    ~Shader();

    void useShaderProgram();
    int getUniformLocation( const char* uniformValue );

  private:
    static unsigned int createAndCompileShader( const char* shaderGLSL, unsigned int shaderType );
    void createShaderProgram( unsigned int vertexShader, unsigned int fragmentShader );

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif // LEARNOPENGL_SHADER_H
