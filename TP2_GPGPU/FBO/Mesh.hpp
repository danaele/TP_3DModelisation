#pragma once

#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include "gl-utils.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "matrix.h"


struct MeshInfo
{
  GLuint vao;
  GLuint vbos[3];
  GLuint vboi;
  int nb_indices;
};


struct Mesh
{
  Mesh( );
  Mesh( std::vector<vec3> positions,
        std::vector<vec3> normals,
        std::vector<vec2> uvs,
        std::vector<GLushort> indices );
  
  
  void LoadFromOBJ( const std::string filename );
  void FillVBOs( MeshInfo* info );
  void Draw( const MeshInfo& info );
  
  
  std::vector<vec3> positions;
  std::vector<vec3> normals;
  std::vector<vec2> uvs;
  std::vector<GLushort> indices;
};


#endif
