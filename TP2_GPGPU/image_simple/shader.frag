#version 130

uniform sampler2D texture_id;
uniform vec4      moyenne = vec4(0.5, 0.5, 0.5, 1.0);
uniform float     alpha;

in vec2 v_tex_coord;

out vec4 color;

void main(void)
{
  // Changement de contraste
  vec4 c = texture(texture_id, v_tex_coord);
  color  = mix(moyenne, c, alpha); 
}
