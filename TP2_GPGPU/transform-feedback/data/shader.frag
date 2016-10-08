#version 130

out vec4 color;

in vec3 distanceColor;


void main (void)
{
  color = vec4(distanceColor, 1.0);
}
