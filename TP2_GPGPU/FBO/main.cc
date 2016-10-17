// main.cc

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gl-utils.h"
#include "matrix.h"

#include "TextureTGA.hpp"
#include "Mesh.hpp"

static float angle_y  = 0.0f;

static GLuint  program;
static GLuint fbo;

Mesh meshTV;
Mesh meshTVScreen = Mesh
  ( 
  { vec3(-0.6f, -0.45f, 0.0f), vec3( -0.6f, 0.3f, 0.0f), vec3( 0.4f, -0.45f, 0.0f), vec3(0.4f, 0.3f, 0.0f) },
  { vec3(-0.6f, 0.0f, 0.0f), vec3( -0.6f, 0.0f, 0.0f), vec3( 0.4f, 0.0f, 0.0f), vec3(0.4f, 0.0f, 0.0f) },
  { vec2(0,1), vec2(0,0), vec2(1,1), vec2(1,0) },
  { 0, 1, 2,
    2, 1, 3 }
  );

Mesh meshBackground = Mesh
  ( 
  { vec3(-1.0f, -1.0f, -1.0f), vec3( -1.0f, 1.0f, -1.0f), vec3( 1.0f, -1.0f, -1.0f), vec3(1.0f, 1.0f, -1.0f) },
  { vec3(-1.0f, -1.0f, 0.0f), vec3( -1.0f, 1.0f, 0.0f), vec3( 1.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },
  { vec2(0,1), vec2(0,0), vec2(1,1), vec2(1,0) },
  { 0, 1, 2,
    2, 1, 3 }
  );

Mesh meshWall = Mesh
  (
  { vec3(-1,-1, -1), vec3( 1,-1, -1), vec3( 1, 1, -1), vec3(-1, 1, -1) ,
    vec3(-1,-1,  1), vec3( 1,-1,  1), vec3( 1, 1,  1), vec3(-1, 1,  1) ,
    vec3(-1,-1, -1), vec3(-1,-1,  1), vec3(-1, 1,  1), vec3(-1, 1, -1) ,
    vec3( 1,-1, -1), vec3( 1,-1,  1), vec3( 1, 1,  1), vec3( 1, 1, -1) ,
    vec3(-1,-1, -1), vec3(-1,-1,  1), vec3( 1,-1,  1), vec3( 1,-1, -1) ,
    vec3(-1, 1, -1), vec3(-1, 1,  1), vec3( 1, 1,  1), vec3( 1, 1, -1) },
  { vec3(-1,-1, -1), vec3( 1,-1, -1), vec3( 1, 1, -1), vec3(-1, 1, -1) ,
    vec3(-1,-1,  1), vec3( 1,-1,  1), vec3( 1, 1,  1), vec3(-1, 1,  1) ,
    vec3(-1,-1, -1), vec3(-1,-1,  1), vec3(-1, 1,  1), vec3(-1, 1, -1) ,
    vec3( 1,-1, -1), vec3( 1,-1,  1), vec3( 1, 1,  1), vec3( 1, 1, -1) ,
    vec3(-1,-1, -1), vec3(-1,-1,  1), vec3( 1,-1,  1), vec3( 1,-1, -1) ,
    vec3(-1, 1, -1), vec3(-1, 1,  1), vec3( 1, 1,  1), vec3( 1, 1, -1) },
  { vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0),
    vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0),
    vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0),
    vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0),
    vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0),
    vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0) },
  { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9,10, 8,10,11, 12,13,14,12,14,15, 16,17,18,16,18,19, 20,21,22,20,22,23 }

  );


MeshInfo info_Background;
MeshInfo info_TV;
MeshInfo info_TVScreen;
MeshInfo info_Wall;


TextureTGA fboTexture;
TextureTGA imageTexture;
TextureTGA brickTexture;

/*****************************************************************************\
 * display_callback                                                          *
\*****************************************************************************/
static void display_callback()
{
  matrix projection;

  projection.set_perspective(30, 1, 0.1, 20);
  projection.rotate( 180, vec3(0,0,1));
  projection.rotate( -angle_y, vec3(0,1,0));
  projection.translate( vec3(0,0,-2) );
  glUniformMatrix4fv(get_uni_loc(program, "projection"), 1, GL_FALSE, projection.m); PRINT_OPENGL_ERROR();

  //** Draw into the FBO **//
  // Selection du FBO comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, fbo); PRINT_OPENGL_ERROR();
  // Effacement du framebuffer
  glClearColor(0.5, 0.8, 0.9, 1.0); PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();
  // Draw scene
  // Selection de la texture_image comme texture courante:
  glBindTexture( GL_TEXTURE_2D, imageTexture.id );  PRINT_OPENGL_ERROR();
  meshTV.Draw( info_TV );
  meshBackground.Draw( info_Background );
  glBindTexture( GL_TEXTURE_2D, brickTexture.id );  PRINT_OPENGL_ERROR();
//   meshWall.Draw( info_Wall );
  meshTV.Draw( info_TV );


  projection.set_perspective(30, 1, 0.1, 20);
  projection.translate(vec3(0,0,-8));
  glUniformMatrix4fv(get_uni_loc(program, "projection"), 1, GL_FALSE, projection.m); PRINT_OPENGL_ERROR();
  
  //** Draw on the screen **//
  // Selection de la fenêtre comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, 0); PRINT_OPENGL_ERROR();
  // Effacement du buffer
  glClearColor(1.0, 1.0, 1.0, 1.0); PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();
  // Draw scene
  // Selection de la texture du fbo comme texture courante.
  glBindTexture( GL_TEXTURE_2D, fboTexture.id );  PRINT_OPENGL_ERROR();
  meshTVScreen.Draw( info_TVScreen );
  // Selection de la texture image comme texture courante.
  glBindTexture( GL_TEXTURE_2D, brickTexture.id );  PRINT_OPENGL_ERROR();
  meshTV.Draw( info_TV );
  
  glutSwapBuffers();
}

/*****************************************************************************\
 * init                                                                      *
\*****************************************************************************/
static void init()
{
  // Load scene
  meshTV.LoadFromOBJ( "TV.obj" );
  meshTV.FillVBOs( &info_TV );
  
  meshTVScreen.FillVBOs( &info_TVScreen );
  meshBackground.FillVBOs( &info_Background );
  meshWall.FillVBOs( &info_Wall );
  
  // Creating shader.
  program = read_shader("shader.vert", "shader.frag", {"position","normal","tex_coord"});
  glUniform1i (get_uni_loc(program, "texture_id"), 0); PRINT_OPENGL_ERROR();

  // Loading texture.
  imageTexture.Load( "texture.tga" );
  brickTexture.Load( "brick.tga" );

  // Enable z-buffer
  glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
  
  //** Creating FBO **//
  // Creation d'un objet framebuffer: fbo
  glGenFramebuffers(1, &fbo);                                                PRINT_OPENGL_ERROR();
  // Selection de ce fbo comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
  // Paramétrisation d'une texture de 800 par 800: fboTexture
  fboTexture.Load( 800, 800, 0 );
  // Attachement de cette texture au FBO.
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture.id, 0 );
  
  // Partie "tampon de profondeur du fbo".
  GLuint depth_render_buffer;
  // Création d'un buffer de rendu : depth_render_buffer
  glGenRenderbuffers( 1, &depth_render_buffer );
  // Selection de ce buffer de rendu comme "Renderbuffer" courant
  glBindRenderbuffer( GL_RENDERBUFFER, depth_render_buffer );
  // Création d'un espace de stockage de 800x800 pixels
  glRenderbufferStorage( GL_RENDERBUFFER,     //Target must be GL_RENDERBUFFER
  	                 GL_DEPTH_COMPONENT24,//Type : Depth on 24bits
  	                 800,                 //Width in px                     
  	                 800 );               //Height in px
  // Attachement de ce tampon de profondeur au FBO courant :
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);

  // Selection de la fenêtre comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, 0); PRINT_OPENGL_ERROR();
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
  switch (key)
  {
    case GLUT_KEY_LEFT:
     angle_y -= 0.1f;
      break;
    case GLUT_KEY_RIGHT:
     angle_y += 0.1f;
      break;
  }
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
  glutCreateWindow("FBO");
  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewInit();
  init();

  glutMainLoop();

  return 0;
}

