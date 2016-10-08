#version 130

uniform sampler2D texture_id;

in vec2 v_tex_coord;
out vec4 color;

void main(void)
{
  color = texture(texture_id, v_tex_coord);
}
