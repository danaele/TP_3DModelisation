#version 130

uniform sampler2D texture_id;

in vec2 v_tex_coord;
in vec3 v_position;
in vec3 v_normal;
out vec4 color;

vec3 lightPos = vec3(20.0);
vec3 cameraPos = vec3(0);

vec3 diffuseColor = vec3( 1 );
float exponant = 28;

void main(void)
{
  vec3 normal = normalize( v_normal );
  vec3 l = normalize( lightPos - v_position );
  vec3 v = normalize( cameraPos - v_position );
  vec3 r = reflect( l, normal );
  
  vec4 v_color = texture(texture_id, v_tex_coord);
//   vec4 v_color = vec4(0.25,0.2,0.2,1);
  
  vec3 normal_color = vec3(0.0);
  normal_color += diffuseColor * max( 0.0, dot( normal, l ) );
  normal_color += vec3(1.0) * pow(max(0.0, dot(r,v)), exponant);
  
  color = v_color /*+  vec4(normal_color, 1.0)*/;
}
