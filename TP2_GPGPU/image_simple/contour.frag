#version 130

uniform sampler2D texture_id;
uniform vec2      size;

in vec2 v_tex_coord;

out vec4 color;

void main(void)
{
  // ** Edges detection using Sobel filter **
  vec4 textureColor = texture(texture_id, v_tex_coord);
  float grayscaleValue = ( textureColor.x + textureColor.y + textureColor.z )/3.0;
  // 3*3 Sobel filters along X and Y
  mat3 Gx = mat3( -1, 0, 1,
                  -2, 0, 2,
                  -1, 0, 1 );
                  
  mat3 Gy = mat3( 1, 2, 1,
                  0, 0, 0,
                 -1, -2, -1 );

  vec2 G = vec2(0);// Image Gradient value for the current pixel
  // Apply Sobel filters to the 3*3 image patch which center is the current pixel
  for (int i=0; i<3; i++)
    {
    for (int j=0; j<3; j++)
      {
      vec3 sample  = texture( texture_id, v_tex_coord + vec2(i-1,j-1)/size ).rgb;
      float sampleValue = ( sample.x + sample.y + sample.z )/3.0; 
      G.x += sampleValue * Gx[i][j];
      G.y += sampleValue * Gy[i][j];
      }
    }
  // The final color is the gradient norm
  color = vec4( vec3( sqrt(dot(G,G)) ), 1.0 );
  
}
