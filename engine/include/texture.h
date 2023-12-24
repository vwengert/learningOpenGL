#ifndef LEARNINGOPENGL_TEXTURE_H
#define LEARNINGOPENGL_TEXTURE_H

#include <memory>
class Texture
{
  public:
    explicit  Texture(const char* image );
    ~Texture();

    void use();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif // LEARNINGOPENGL_TEXTURE_H
