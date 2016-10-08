// main.cc

#include <iostream>
#include <cstdlib>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gl-utils.h"
#include "matrix.h"
#include "image.h"

static float angle  = 0;
static GLuint  program;
static GLuint fbo, fbo_texture;
static GLuint texture_image;

/*****************************************************************************\
 * display_callback                                                          *
\*****************************************************************************/
static void display_callback()
{
  matrix projection;

  angle += 1;
  projection.set_perspective(30, 1, 0.1, 20);
  projection.translate(vec3(0,0,-8));
  projection.rotate(angle, vec3(1,1,0));
  glUniformMatrix4fv(get_uni_loc(program, "projection"), 1, GL_FALSE, projection.m); PRINT_OPENGL_ERROR();

  // Draw into the FBO.
  // *** Selection du FBO comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, fbo); PRINT_OPENGL_ERROR();
  // Selection de la texture_image comme texture courante:
  glBindTexture(GL_TEXTURE_2D, texture_image);  PRINT_OPENGL_ERROR();
  // *** Effacement du framebuffer
  // *** Dessin du cube
  glClearColor(0.5, 0.8, 0.9, 1.0); PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0); PRINT_OPENGL_ERROR();

  // Draw on the screen.
  // *** Selection de la fenêtre comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, 0); PRINT_OPENGL_ERROR();
  // *** Selection de la texture du fbo comme texture courante.
  glBindTexture(GL_TEXTURE_2D, fbo_texture);  PRINT_OPENGL_ERROR();

  glClearColor(1.0, 1.0, 1.0, 1.0); PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0); PRINT_OPENGL_ERROR();

  glutSwapBuffers();
}

/*****************************************************************************\
 * init                                                                      *
\*****************************************************************************/
static void init()
{
  GLuint vao, vbo, vboi;
  // Creating geometry.
  struct Data{vec3 position;vec2 tex_coord;} data[] =
  {
    { vec3(-1,-1, -1), vec2(0,1) }, { vec3( 1,-1, -1), vec2(1,1) }, { vec3( 1, 1, -1), vec2(1,0) }, { vec3(-1, 1, -1), vec2(0,0) },
    { vec3(-1,-1,  1), vec2(0,1) }, { vec3( 1,-1,  1), vec2(1,1) }, { vec3( 1, 1,  1), vec2(1,0) }, { vec3(-1, 1,  1), vec2(0,0) },
    { vec3(-1,-1, -1), vec2(0,1) }, { vec3(-1,-1,  1), vec2(1,1) }, { vec3(-1, 1,  1), vec2(1,0) }, { vec3(-1, 1, -1), vec2(0,0) },
    { vec3( 1,-1, -1), vec2(0,1) }, { vec3( 1,-1,  1), vec2(1,1) }, { vec3( 1, 1,  1), vec2(1,0) }, { vec3( 1, 1, -1), vec2(0,0) },
    { vec3(-1,-1, -1), vec2(0,1) }, { vec3(-1,-1,  1), vec2(1,1) }, { vec3( 1,-1,  1), vec2(1,0) }, { vec3( 1,-1, -1), vec2(0,0) },
    { vec3(-1, 1, -1), vec2(0,1) }, { vec3(-1, 1,  1), vec2(1,1) }, { vec3( 1, 1,  1), vec2(1,0) }, { vec3( 1, 1, -1), vec2(0,0) }
  };
  GLushort indices[] =
  { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9,10, 8,10,11, 12,13,14,12,14,15, 16,17,18,16,18,19, 20,21,22,20,22,23 };
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);                                                          PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);                                             PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);               PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(0);                                                   PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), 0);               PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(1);                                                   PRINT_OPENGL_ERROR();
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), BUFFER_OFFSET(sizeof(vec3)));  PRINT_OPENGL_ERROR();
  // Index
  glGenBuffers(1, &vboi);                                                         PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);                                    PRINT_OPENGL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW); PRINT_OPENGL_ERROR();
  // Creating shader.
  program = read_shader("shader.vert", "shader.frag", {"position", "tex_coord"});
  glUniform1i (get_uni_loc(program, "texture_id"), 0); PRINT_OPENGL_ERROR();

  // Loading texture.
  Image  *image = image_load_tga("texture.tga");
  if (image)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                              PRINT_OPENGL_ERROR();
    glGenTextures(1, &texture_image);                                   PRINT_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_image);                        PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   PRINT_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);             PRINT_OPENGL_ERROR();
    delete image;
  }
  else
    abort();
  glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
  // FBO
  // *** Creation d'un objet framebuffer: fbo
  glGenFramebuffers(1, &fbo);                                                PRINT_OPENGL_ERROR();
  // *** Selection de ce fbo comme framebuffer courant
  glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
  // *** Création et paramétrisation d'une texture de 800 par 800: fbo_texture
  glGenTextures(1, &fbo_texture);
  glBindTexture( GL_TEXTURE_2D, fbo_texture );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   PRINT_OPENGL_ERROR();
  glTexImage2D( GL_TEXTURE_2D, 0 ,GL_RGBA, 800, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0 );
  // *** Attachement de cette texture au FBO.
  //
  // Partie "tampon de profondeur du fbo".
  GLuint depth_render_buffer;
  // *** Création d'un buffer de rendu (voir glGenRenderbuffers): depth_render_buffer
  glGenRenderbuffers( 1, &depth_render_buffer );
  // *** Selection de ce buffer de rendu comme "Renderbuffer" courant
  glBindRenderbuffer( GL_RENDERBUFFER, depth_render_buffer );
  // *** Création d'un espace de stockage de 800x800 pixels de type "profondeur sur 24 bits" (GL_DEPTH_COMPONENT24)
  glRenderbufferStorage( GL_RENDERBUFFER,
  	GL_DEPTH_COMPONENT24,
  	800,
  	800 );
  // attachement de ce tampon de profondeur au FBO courant :
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);

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
    case GLUT_KEY_UP:
//      angle1 += 0.1f;
      break;
    case GLUT_KEY_DOWN:
//      angle1 -= 0.1f;
      break;
    case GLUT_KEY_LEFT:
//      angle2 += 0.1f;
      break;
    case GLUT_KEY_RIGHT:
//      angle2 -= 0.1f;
      break;
  }
  glutPostRedisplay();
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

