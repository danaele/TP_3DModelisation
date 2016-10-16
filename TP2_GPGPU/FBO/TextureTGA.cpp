#include "TextureTGA.hpp"

TextureTGA::TextureTGA()
{
}

void TextureTGA::Load( int width, int height, const GLvoid * data)
{
  glGenTextures(1, &id);                                   PRINT_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, id);                        PRINT_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       PRINT_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       PRINT_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   PRINT_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   PRINT_OPENGL_ERROR();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
               0, GL_RGB, GL_UNSIGNED_BYTE, data);             PRINT_OPENGL_ERROR();
}
  
void TextureTGA::Load( const std::string &filename )
{
  Image *image = image_load_tga( filename );
  if( image )
    {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                              PRINT_OPENGL_ERROR();
    Load( image->width, image->height, image->data );
    delete image;
    }
  else
    {
    abort();  
    }
}