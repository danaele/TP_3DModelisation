#version 130

in vec3 position;
in vec3 vitesse;
in vec3 random_seed;

out vec3 v_position;
out vec3 v_vitesse;

uniform vec3 center;

void main (void)
{
// ** Update vitesse **
  v_vitesse = vitesse;
  // Attract particle to the center
  if( length( center - position ) < 0.2  )
    v_vitesse = 2.0 * ( center - position );
  // Keep the particle in the sphere with random and rotation movements 
  if( length( center - position ) < 0.15 )  
     v_vitesse =  vitesse + 0.005 * random_seed + 0.02 * cross( ( center - position ), vec3(0,0,1) );   

// ** Update position **
  v_position = position + v_vitesse / 100.0;
  // Keep particle in the screen
  if ( abs(v_position.x) > 1.0) v_position.x *= -1.0;
  if ( abs(v_position.y) > 1.0) v_position.y *= -1.0;

  gl_Position = vec4(position, 1.0);
}
