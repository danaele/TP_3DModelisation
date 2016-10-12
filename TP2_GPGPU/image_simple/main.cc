// main.cc

#include <iostream>
#include <cstdlib>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gl-utils.h"
#include "image.h"

static GLuint  program;
static Image  *image;
static float   alpha = 0.5f;
// Shader ID to perform different image processing
enum ShaderID { contrast, threshold, saturation, contour, blur };
ShaderID shaderID = contrast; //Default value is the original contrast modifier from the subject

struct vec2
{
  GLfloat x, y;
  vec2(float x=0,float y=0):x(x),y(y){}
};

/*****************************************************************************\
 * update_uniforms                                                           *
\*****************************************************************************/
static void update_uniforms()
{ 
  switch( shaderID )
    {
    case contrast:
    case threshold:
    case saturation:
      glUniform1f(get_uni_loc(program, "alpha"), alpha); PRINT_OPENGL_ERROR();
      break;
    case contour:
      glUniform2f(get_uni_loc(program, "size"), 800, 450); PRINT_OPENGL_ERROR();
      break;
    case blur:
      glUniform2f(get_uni_loc(program, "size"), 800, 450); PRINT_OPENGL_ERROR();
      break;
    }
}

/*****************************************************************************\
 * display_callback                                                          *
\*****************************************************************************/
static void display_callback()
{
  update_uniforms();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); PRINT_OPENGL_ERROR();

  glutSwapBuffers ();
}

/*****************************************************************************\
 * init                                                                      *
\*****************************************************************************/
static void init()
{
  GLuint vao, vbo, vboi;
  // Creating geometry.
  vec2 data[] =
  {
    vec2(-1,-1), vec2(0,1),
    vec2( 1,-1), vec2(1,1),
    vec2( 1, 1), vec2(1,0),
    vec2(-1, 1), vec2(0,0)
  };
  GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);                                                          PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);                                             PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);               PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(0);                                                   PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec2), 0);           PRINT_OPENGL_ERROR();
  glEnableVertexAttribArray(1);                                                   PRINT_OPENGL_ERROR();
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec2), BUFFER_OFFSET(sizeof(vec2)));  PRINT_OPENGL_ERROR();
  // Index
  glGenBuffers(1, &vboi);                                                         PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);                                    PRINT_OPENGL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW); PRINT_OPENGL_ERROR();
  // Creating default shader.
  program = read_shader("shader.vert", "shader.frag", {"position", "tex_coord"});
  glUniform1i (get_uni_loc(program, "texture_id"), 0); PRINT_OPENGL_ERROR();

  // Loading texture.
  image = image_load_tga("texture.tga");
  if (image)
  {
    GLuint texture_id;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                                 PRINT_OPENGL_ERROR();
    glGenTextures(1, &texture_id);                                         PRINT_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id);                              PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);          PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);          PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);      PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);      PRINT_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);                PRINT_OPENGL_ERROR();
    delete image;
  }
  else
    abort();
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
      break;
    // shaderID selection
    case '0'://Contrast (Default)
      program = read_shader("shader.vert", "shader.frag", {"position", "tex_coord"});
      shaderID = threshold;
      break;
    case '1'://Thresholding
      program = read_shader("shader.vert", "threshold.frag", {"position", "tex_coord"});
      shaderID = contrast;
      break;
    case '2'://Saturation
      program = read_shader("shader.vert", "saturation.frag", {"position", "tex_coord"});
      shaderID = saturation;
      break;
    case '3'://Edges detection
      program = read_shader("shader.vert", "contour.frag", {"position", "tex_coord"});
      shaderID = contour;
      break;
    case '4'://Blur
      program = read_shader("shader.vert", "blur.frag", {"position", "tex_coord"});
      shaderID = blur;
      break;
  }
  glUniform1i( get_uni_loc(program, "texture_id"), 0 ); PRINT_OPENGL_ERROR();
  glutPostRedisplay();
}

/*****************************************************************************\
 * special_callback                                                          *
\*****************************************************************************/
static void special_callback(int key, int,int)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      alpha += 0.1f;
      break;
    case GLUT_KEY_DOWN:
      alpha -= 0.1f;
      break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInitContextVersion(3, 0); //OpenGL 3.0 required
//  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
//  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 450);
  glutCreateWindow("Image Simple");
  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);

  glewInit();
  init();

  glutMainLoop();

  return 0;
}

