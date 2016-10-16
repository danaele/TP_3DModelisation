#include "Mesh.hpp"

Mesh::Mesh()
{
}

Mesh::Mesh( std::vector<vec3> positions_param,
            std::vector<vec3> normals_param,
            std::vector<vec2> uvs_param,
            std::vector<GLushort> indices_param )
{
  positions = positions_param;
  normals = normals_param;
  uvs = uvs_param;
  indices = indices_param;
}

void Mesh::LoadFromOBJ( const std::string filename )
{
  positions.push_back(vec3(0,0,0));
  normals.push_back(vec3(0,0,0));
  uvs.push_back(vec2(0,0));
  std::ifstream file( filename );
  while (file)
    {
    std::string line, tag;
    getline(file, line);
    std::stringstream ss(line);
    ss >> tag;
    if (tag == "v")
      {
      float x, y, z;
      ss >> x >> y >> z;
      positions.push_back(vec3(x, y-0.5f, z-0.36f));
      }
    else if (tag == "vn")
      {
      float x, y, z;
      ss >> x >> y >> z;
      normals.push_back(vec3(x, y, z));
      }
    else if (tag == "vt")
      {
      float u, v;
      ss >> u >> v;
      uvs.push_back(vec2(u, v));
      }
    else if (tag == "f")
      {
      short index;
      ss >> index;
      indices.push_back(index);
      ss >> index;
      indices.push_back(index);
      ss >> index;
      indices.push_back(index);
      }
    }
}

void Mesh::FillVBOs( MeshInfo* info )
{
  glGenVertexArrays(1, &info->vao);
  glBindVertexArray(info->vao);
  
  glGenBuffers(3, info->vbos);                                                                           PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, info->vbos[0]);                                                          PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), &positions[0], GL_STATIC_DRAW);   PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(0);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, info->vbos[1]);                                                          PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);       PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(1);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, info->vbos[2]);                                                          PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW); PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(2);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glGenBuffers(1, &info->vboi);                                                                          PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info->vboi);                                                     PRINT_OPENGL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(short), &indices[0], GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

  info->nb_indices = indices.size();
  
}

void Mesh::Draw( const MeshInfo& info )
{
  glBindBuffer(GL_ARRAY_BUFFER, info.vbos[0]);                                                          PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(0);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, info.vbos[1]);                                                          PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(1);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, info.vbos[2]);                                                          PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(2);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.vboi);                                                     PRINT_OPENGL_ERROR();
  
  glDrawElements(GL_TRIANGLES, info.nb_indices, GL_UNSIGNED_SHORT, 0); PRINT_OPENGL_ERROR();

}