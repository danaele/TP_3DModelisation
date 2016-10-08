// main.cc

#include <iostream>
#include <cstdlib>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gl-utils.h"
#include "matrix.h"
#include "image.h"

const int NB_PARTICULES = 500;

enum
{
  POSITION0,
  POSITION1,
  VITESSE0,
  VITESSE1,
  RANDOM_SEED
};

static GLuint query;
static GLuint buffers[5]; // Les VBO pour les positions, directionpenser à en ajouter pour les vitesses.

static GLuint  program_tf;
static GLuint  program;

vec3 center( 0.0f, 0.0f, 0.0f );

/*****************************************************************************\
 * init                                                                      *
 * initialise le tout, une fois pour toute.                                  *
\*****************************************************************************/
static void init()
{
  vec3 random_seeds[NB_PARTICULES]; // Initial random seeds array
  vec3 positions[NB_PARTICULES]; // Initial positions array
  const char *varyings[2] = { "v_position", "v_vitesse" };

  program_tf = read_shader("data/tf.vert", "data/tf.frag");
  glTransformFeedbackVaryings(program_tf, 2, varyings, GL_SEPARATE_ATTRIBS); PRINT_OPENGL_ERROR();
  glBindAttribLocation(program_tf, 0, "position");                           PRINT_OPENGL_ERROR();
  glBindAttribLocation(program_tf, 1, "vitesse");                            PRINT_OPENGL_ERROR();
  glBindAttribLocation(program_tf, 2, "random_seed");                            PRINT_OPENGL_ERROR();
  glLinkProgram(program_tf);                                                 PRINT_OPENGL_ERROR();

  program = read_shader("data/shader.vert", "data/shader.frag");
  glBindAttribLocation(program, 0, "position"); PRINT_OPENGL_ERROR();
  glLinkProgram(program);                       PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(0); PRINT_OPENGL_ERROR();

  // L'objet query
  glGenQueries(1, &query);

  for (int i = 0; i < NB_PARTICULES; i++)
    {
    random_seeds[i] = vec3(1.0-2*double(rand())/RAND_MAX,1.0-2*double(rand())/RAND_MAX);
    positions[i] = vec3(1.0-2*double(rand())/RAND_MAX,1.0-2*double(rand())/RAND_MAX); 
    }

  // Les deux VBO pour les positions, à adapter pour les vitesses
  glGenBuffers(5, buffers);                                                      PRINT_OPENGL_ERROR();
  
  glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION0]);                             PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof positions, positions, GL_STREAM_DRAW);    PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION1]);                             PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof positions, NULL, GL_STREAM_DRAW);         PRINT_OPENGL_ERROR();

  glEnableVertexAttribArray(1); PRINT_OPENGL_ERROR();// WARNING: Essential, DO NOT REMOVE. => ramener celui pour 0 avant les positions? Generer des buffers independants?.
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VITESSE0]);                              PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof random_seeds, random_seeds, GL_STREAM_READ);      PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VITESSE1]);                              PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof random_seeds, NULL, GL_STREAM_READ);      PRINT_OPENGL_ERROR();

  glEnableVertexAttribArray(2); PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, buffers[RANDOM_SEED]);                              PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof random_seeds, random_seeds, GL_STREAM_READ); PRINT_OPENGL_ERROR();
  // Prepare random data
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);                         PRINT_OPENGL_ERROR();
  
  
  glPointSize(5);                                                                PRINT_OPENGL_ERROR();
  glClearColor(0.0, 0.0, 0.3, 1.0); PRINT_OPENGL_ERROR();                        PRINT_OPENGL_ERROR();
}

/*****************************************************************************\
 * update_particles                                                          *
 * appelé régulièrement, pour mettre à jour la position (et la vitesse) des  *
 * particules                                                                *
\*****************************************************************************/
static GLuint update_particles()
{
  GLuint written_particles;
  // On débranche la fin du pipeline
  glEnable(GL_RASTERIZER_DISCARD);                                               PRINT_OPENGL_ERROR();
  glUseProgram(program_tf);                                                      PRINT_OPENGL_ERROR();
  //Update uniforms
  glUniform3f( get_uni_loc(program_tf, "center"), center.x, center.y, center.z ); PRINT_OPENGL_ERROR();

  // Préparation pour les positions
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffers[POSITION1]);         PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION0]);                             PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);                         PRINT_OPENGL_ERROR();
  // Préparation pour les vitesses
//    glEnableVertexAttribArray(1); //WARNING
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, buffers[VITESSE1]);          PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VITESSE0]);                              PRINT_OPENGL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);                         PRINT_OPENGL_ERROR();
  
  // C'est parti !
  // On active les attributs "position" et "vitesse"
  glEnableVertexAttribArray(0);                                                  PRINT_OPENGL_ERROR();
  // Activation de l'écriture dans le VBO potision.
  glBeginTransformFeedback(GL_POINTS);                                           PRINT_OPENGL_ERROR();
  // Activation du comptage des "points"
  glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);                 PRINT_OPENGL_ERROR();
  // Pseudo dessin
  glDrawArrays(GL_POINTS, 0, NB_PARTICULES);                                     PRINT_OPENGL_ERROR();
  glEndTransformFeedback();                                                      PRINT_OPENGL_ERROR();
  glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);                          PRINT_OPENGL_ERROR();
  glGetQueryObjectuiv(query, GL_QUERY_RESULT, &written_particles);               PRINT_OPENGL_ERROR();
  glDisableVertexAttribArray(0);                                                 PRINT_OPENGL_ERROR();
 //WARNING
/*  glEnableVertexAttribArray(1);                                                  PRINT_OPENGL_ERROR();
  // Activation de l'écriture dans le VBO potision.
  glBeginTransformFeedback(GL_POINTS);                                           PRINT_OPENGL_ERROR();
  // Pseudo dessin
  glDrawArrays(GL_POINTS, 0, NB_PARTICULES);                                     PRINT_OPENGL_ERROR();
  glEndTransformFeedback();                                                      PRINT_OPENGL_ERROR();
  glDisableVertexAttribArray(1);  */                                               PRINT_OPENGL_ERROR();
 
  // On echange les tampons d'écriture et de lecture.
  std::swap(buffers[POSITION0], buffers[POSITION1]);
  std::swap(buffers[VITESSE0], buffers[VITESSE1]);
  // On rebranche la fin du pipeline
  glDisable(GL_RASTERIZER_DISCARD);                                              PRINT_OPENGL_ERROR();

  return written_particles;
}

/*****************************************************************************\
 * display_callback                                                          *
\*****************************************************************************/
static void display_callback()
{
  matrix projection;
  GLuint count = update_particles();
  
  // Get vitesses
  vec3 vitesses[NB_PARTICULES];
  // WARNING: Ici on accede bien aux vitesses car c'est le T_F_BUFFER courant.
  glGetBufferSubData( GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof vitesses, vitesses );  PRINT_OPENGL_ERROR();

  
//  angle += 1;
//  projection.set_perspective(30, 1, 0.1, 20);
//  projection.translate(vec3(0,0,-8));
//  projection.rotate(angle, vec3(1,1,0));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                                PRINT_OPENGL_ERROR();
  glUseProgram(program);                                                             PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(program, "projection"), 1, GL_FALSE, projection.m); PRINT_OPENGL_ERROR();
  glUniform3f( get_uni_loc(program, "center"), center.x, center.y, center.z ); PRINT_OPENGL_ERROR();


  glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION0]);     PRINT_OPENGL_ERROR();
  // On active les attributs "position"
  glEnableVertexAttribArray(0);                                                  PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); PRINT_OPENGL_ERROR();
  glDrawArrays(GL_POINTS, 0, count);                     PRINT_OPENGL_ERROR();
  glDisableVertexAttribArray(0);                                                 PRINT_OPENGL_ERROR();

  glutSwapBuffers();
}

/*****************************************************************************\
 * keyboard_callback                                                         *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'q':
    case 'Q':
    case 27:
      exit(0);
  }
}

/*****************************************************************************\
 * special_callback                                                          *
\*****************************************************************************/
static void special_callback(int key, int,int)
{
  
  float sensibilite = 1e-2f;
  switch (key)
  {
    case GLUT_KEY_UP:
      center.y += sensibilite;
      break;
    case GLUT_KEY_DOWN:
      center.y -= sensibilite;
      break;
    case GLUT_KEY_LEFT:
      center.x -= sensibilite;
      break;
    case GLUT_KEY_RIGHT:
      center.x += sensibilite;
      break;
  }
  //Clamp center between [-1;1] in X and Y directions
  if ( center.x > 1.0) center.x = 1.0;
  if ( center.x < -1.0) center.x = -1.0;
  if ( center.y > 1.0) center.y = 1.0;
  if ( center.y < -1.0) center.y = -1.0;

  glUseProgram(program);                                                             PRINT_OPENGL_ERROR();
  glUniform3f( get_uni_loc(program, "center"), center.x, center.y, center.z ); PRINT_OPENGL_ERROR();

//   glutPostRedisplay();
}

/*****************************************************************************\
 * timer_callback                                                            *
\*****************************************************************************/
static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInitContextVersion(3, 0);
  //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  //glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Transform feedback");
  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewInit(); PRINT_OPENGL_ERROR();
  init();

  glutMainLoop();

  return 0;
}

