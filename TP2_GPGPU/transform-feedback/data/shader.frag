#version 330

uniform sampler2D text;

in vec2 v_tex_coord;

out vec4 color;


void main (void)
{
  vec4 c = texture(text, v_tex_coord);
  c.w = ( c.x + c.y + c.z )/3.0;
  color = c;
}
