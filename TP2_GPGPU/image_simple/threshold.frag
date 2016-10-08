#version 130

uniform sampler2D texture_id;
uniform float     alpha;

in vec2 v_tex_coord;

out vec4 color;

void main(void)
{
  //  ** Thresholding ** //
  vec4 textureColor = texture(texture_id, v_tex_coord);
  float grayscaleValue = ( textureColor.x + textureColor.y + textureColor.z )/3.0;
  // Thresholding using threshold = alpha/10.0  
  grayscaleValue = float( grayscaleValue > alpha/10.0 );
  color  = vec4( vec3( grayscaleValue ), 1.0 );  
}
