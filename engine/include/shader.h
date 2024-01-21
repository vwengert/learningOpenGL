#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Shader
{
  public:
    explicit Shader( const char* vertexPath, const char* fragmentPath );
    ~Shader();

    void use( glm::mat4 model, glm::mat4 view, glm::mat4 projection ) const;
    [[maybe_unused]] void setBool( const std::string& name, bool value ) const;
    [[maybe_unused]] void setInt( const std::string& name, int value ) const;
    [[maybe_unused]] void setFloat( const std::string& name, float value ) const;
    [[maybe_unused]] int getUniformLocation( const char* uniformValue ) const;

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif // LEARNOPENGL_SHADER_H
