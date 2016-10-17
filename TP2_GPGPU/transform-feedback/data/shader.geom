#version 330

layout(points) in;
layout(triangle_strip,max_vertices=4) out;

uniform mat4 projection;

in vec3 v_position[];
out vec2 v_tex_coord;


void main(void)
{
  
  float size = 0.05;//sprite size
  // Create sprite
  vec3 v0 = v_position[0] + size * vec3(-1,-1,0);
  vec3 v1 = v_position[0] + size * vec3(-1,1,0);
  vec3 v2 = v_position[0] + size * vec3(1,-1,0);
  vec3 v3 = v_position[0] + size * vec3(1,1,0);
  
  gl_Position = projection * vec4(v0, 1.0);
  v_tex_coord = vec2(0,1)/3;
  EmitVertex();

  gl_Position = projection * vec4(v1, 1.0);
  v_tex_coord = vec2(0,0)/3;
  EmitVertex();

  gl_Position = projection * vec4(v2, 1.0);
  v_tex_coord = vec2(1,1)/3;
  EmitVertex();
  
  gl_Position = projection * vec4(v3, 1.0);
  v_tex_coord = vec2(1,0)/3;
  EmitVertex();

  EndPrimitive();
  



}
