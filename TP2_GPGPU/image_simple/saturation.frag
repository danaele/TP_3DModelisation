#version 130

uniform sampler2D texture_id;
uniform float     alpha;

in vec2 v_tex_coord;

out vec4 color;

void main(void)
{
  // ** Saturation **
  vec4 textureColor = texture(texture_id, v_tex_coord);
  float grayscaleValue = ( textureColor.x + textureColor.y + textureColor.z )/3.0;
  vec4 grayscaleColor = vec4( grayscaleValue, grayscaleValue, grayscaleValue, 1.0 );
  //Clamp alpha in [0.0,1.0]
  if( alpha < 0.0 )
   color = mix( grayscaleColor, textureColor, 0.0 );
  else
   color = mix( grayscaleColor, textureColor, alpha );
}
