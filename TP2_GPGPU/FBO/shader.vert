#version 130

uniform mat4 projection;

in vec3 position;
in vec2 tex_coord;
out vec2 v_tex_coord;

void main(void)
{
  gl_Position = projection * vec4(position, 1.0);
  v_tex_coord = tex_coord;
}
