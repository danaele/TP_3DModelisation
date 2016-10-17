#pragma once

#ifndef TEXTURETGA_HPP
#define TEXTURETGA_HPP

#include <GL/glew.h>
#include "gl-utils.h"

#include <string>

#include "image.h"

struct TextureTGA
{
  TextureTGA();
  
  void Load( int width, int height, const GLvoid * data);
  void Load( const std::string &filename );
  
  GLuint id;
};


#endif
