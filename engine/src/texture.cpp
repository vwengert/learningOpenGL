#include "texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h"
#include "stb_image.h"

class Texture::PrivateData
{
  public:
    unsigned int texture{ 0 };
};

Texture::Texture( const char* image )
  : m_data{ new PrivateData }
{
  int width, height, nrChannels;
  unsigned char* data = stbi_load( image, &width, &height, &nrChannels, 0 );
  if( !data )
  {
    std::cout << "Failed to load the texture " << '\n';
    return;
  }
  glGenTextures( 1, &m_data->texture );
  glBindTexture( GL_TEXTURE_2D, m_data->texture );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
  glGenerateMipmap( GL_TEXTURE_2D );
  stbi_image_free( data );
}
void Texture::use()
{
  if( m_data->texture )
  {
    glBindTexture( GL_TEXTURE_2D, m_data->texture );
  }
}

Texture::~Texture() = default;
