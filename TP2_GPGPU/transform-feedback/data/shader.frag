#version 330

uniform sampler2D text;

in vec2 v_tex_coord;

out vec4 color;


void main (void)
{
  color = texture(text, v_tex_coord);
}
