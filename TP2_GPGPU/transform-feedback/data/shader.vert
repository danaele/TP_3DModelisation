#version 130

uniform mat4 projection;
uniform vec3 center;

in vec3 position;

out vec3 distanceColor;


void main (void)
{
  distanceColor = vec3( 1.0, 1.5 * length( center - position ), 0.0 );

  gl_Position = projection * vec4(position, 1.0);
}
