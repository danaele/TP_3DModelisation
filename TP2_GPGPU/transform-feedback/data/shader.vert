#version 330

uniform mat4 projection;

in vec3 position;

out vec3 v_position;

void main (void)
{
  v_position = position;
  gl_Position = projection * vec4(position, 1.0);
}
