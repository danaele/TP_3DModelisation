#version 130
uniform sampler2D texture_id;
uniform vec2      size;

in vec2 v_tex_coord;
out vec4 color;

void main(void)
{
//Flou
  vec2 offset = vec2(1.0,1.0) / size;
  vec4 colorTemp = texture(texture_id, v_tex_coord + offset * vec2(-1.0,-1.0));
  colorTemp = colorTemp + texture(texture_id, v_tex_coord + offset * vec2(-1.0,0.0));
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(-1.0,1.0));
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(0.0,-1.0));  
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(0.0,0.0));
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(0.0,1.0));
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(1.0,-1.0));
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(1.0,0.0));
  colorTemp += texture(texture_id, v_tex_coord + offset * vec2(1.0,1.0));
  
  color = colorTemp / 9.0;
}