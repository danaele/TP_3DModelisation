#version 130

uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

out vec2 v_tex_coord;
out vec3 v_position;
out vec3 v_normal;

void main(void)
{
  gl_Position = projection * vec4(position, 1.0);
  v_position = position;
  v_normal = normal;
  v_tex_coord = tex_coord;
  
}
