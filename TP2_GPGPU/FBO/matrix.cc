// matrix.cc

#include "matrix.h"

static matrix identity;
/******************************************************************************\
 * matrix::matrix                                                             *
\******************************************************************************/
matrix::matrix()
{
  m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = 0;
  m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = 0;
  m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = 0;
  m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

/******************************************************************************\
 * matrix::translate                                                          *
\******************************************************************************/
void matrix::translate(const vec3 &trans)
{
  m[12] = m[0] * trans.x + m[4] * trans.y + m[8]  * trans.z + m[12];
  m[13] = m[1] * trans.x + m[5] * trans.y + m[9]  * trans.z + m[13];
  m[14] = m[2] * trans.x + m[6] * trans.y + m[10] * trans.z + m[14];
  m[15] = m[3] * trans.x + m[7] * trans.y + m[11] * trans.z + m[15];
}

/******************************************************************************\
 * matrix::scale                                                              *
\******************************************************************************/
void matrix::scale(const vec3 &factor)
{
   m[0] *= factor.x;   m[4] *= factor.y;   m[8]  *= factor.z;
   m[1] *= factor.x;   m[5] *= factor.y;   m[9]  *= factor.z;
   m[2] *= factor.x;   m[6] *= factor.y;   m[10] *= factor.z;
   m[3] *= factor.x;   m[7] *= factor.y;   m[11] *= factor.z;
}

/******************************************************************************\
 * matrix::scale                                                              *
\******************************************************************************/
void matrix::scale(float factor)
{
  scale(vec3(factor, factor, factor));
}

/******************************************************************************\
 * matrix::set_perspective                                                    *
\******************************************************************************/
void matrix::set_perspective(float fovy, float aspect, float near, float far)
{
  m[ 0] = 1.0f / tan(fovy * M_PI / 360.0f) / aspect;
  m[ 1] = 0.0f;
  m[ 2] = 0.0f;
  m[ 3] = 0.0f;

  m[ 4] = 0.0f;
  m[ 5] = 1.0f / tan(fovy * M_PI / 360.0f);
  m[ 6] = 0.0f;
  m[ 7] = 0.0f;

  m[ 8] = 0.0f;
  m[ 9] = 0.0f;
  m[10] = (far + near) / (near - far);
  m[11] = -1.0f;

  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 2.0f * far * near / (near - far);
  m[15] = 0.0f;
}

/******************************************************************************\
 * matrix::set_ortho                                                          *
\******************************************************************************/
void matrix::set_ortho(float left, float right, float bottom, float top, float near, float far)
{
#define M(row,col)  m[col * 4 + row]
   M(0,0) = 2.0f / (right - left);
   M(0,1) = 0.0f;
   M(0,2) = 0.0f;
   M(0,3) = -(right + left) / (right - left);

   M(1,0) = 0.0f;
   M(1,1) = 2.0f / (top - bottom);
   M(1,2) = 0.0f;
   M(1,3) = -(top + bottom) / (top - bottom);

   M(2,0) = 0.0f;
   M(2,1) = 0.0f;
   M(2,2) = -2.0f / (far - near);
   M(2,3) = -(far + near) / (far - near);

   M(3,0) = 0.0f;
   M(3,1) = 0.0f;
   M(3,2) = 0.0f;
   M(3,3) = 1.0f;
#undef M
}

/******************************************************************************\
 * matrix::set_look_at                                                        *
\******************************************************************************/
void matrix::set_look_at(const vec3 &eye, const vec3 &center, const vec3 &up)
{
  vec3 f = (center - eye).normalize();
  vec3 my_up = up;
  vec3 s, u;

  my_up.normalize();
  s = f.cross(up);
  u = s.cross(f);

  matrix rotation;

  rotation.m[ 0] = s.x;
  rotation.m[ 1] = u.x;
  rotation.m[ 2] = -f.x;
  rotation.m[ 3] = 0.0f;

  rotation.m[ 4] = s.y;
  rotation.m[ 5] = u.y;
  rotation.m[ 6] = -f.y;
  rotation.m[ 7] = 0.0f;

  rotation.m[ 8] = s.z;
  rotation.m[ 9] = u.z;
  rotation.m[10] = -f.z;
  rotation.m[11] = 0.0f;

  rotation.m[12] = 0;
  rotation.m[13] = 0;
  rotation.m[14] = 0;
  rotation.m[15] = 1.0f;

  matrix translation;

  translation.m[ 0] = 1;
  translation.m[ 1] = 0;
  translation.m[ 2] = 0;
  translation.m[ 3] = 0.0f;

  translation.m[ 4] = 0;
  translation.m[ 5] = 1;
  translation.m[ 6] = 0;
  translation.m[ 7] = 0.0f;

  translation.m[ 8] = 0;
  translation.m[ 9] = 0;
  translation.m[10] = 1;
  translation.m[11] = 0.0f;

  translation.m[12] = -eye.x;
  translation.m[13] = -eye.y;
  translation.m[14] = -eye.z;
  translation.m[15] = 1.0f;

  *this = rotation * translation;
}

/******************************************************************************\
 * matrix::operator*                                                          *
\******************************************************************************/
matrix matrix::operator*(const matrix &mat) const
{
  matrix result;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      {
        result.m[i * 4 + j] = 0.0f;
        for (int k = 0; k < 4; k++)
          result.m[i * 4 + j] += mat.m[i * 4 + k] * m[k * 4 + j];
      }
  return result;
}

/******************************************************************************\
 * matrix::rotate                                                             *
\******************************************************************************/
void matrix::rotate(float angle, const vec3 &v)
{
  float s = sin(angle * M_PI / 180.0);
  float c = cos(angle * M_PI / 180.0);
  float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
  matrix mat;

#define M(row,col)  mat.m[col*4+row]
  if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f)
    return;
  float x = v.x / v.length();
  float y = v.y / v.length();
  float z = v.z / v.length();

  xx = x * x;
  yy = y * y;
  zz = z * z;
  xy = x * y;
  yz = y * z;
  zx = z * x;
  xs = x * s;
  ys = y * s;
  zs = z * s;
  one_c = 1.0f - c;
  M(0,0) = (one_c * xx) + c;
  M(0,1) = (one_c * xy) - zs;
  M(0,2) = (one_c * zx) + ys;
  M(1,0) = (one_c * xy) + zs;
  M(1,1) = (one_c * yy) + c;
  M(1,2) = (one_c * yz) - xs;
  M(2,0) = (one_c * zx) - ys;
  M(2,1) = (one_c * yz) + xs;
  M(2,2) = (one_c * zz) + c;
#undef M

  *this = *this * mat;
}

/******************************************************************************\
 * matrix::transform                                                          *
\******************************************************************************/
vec3 matrix::transform(const vec3 &in)
{
  return vec3(in.x * m[ 0] + in.y * m[ 4] + in.z * m[ 8] + m[12],
              in.x * m[ 1] + in.y * m[ 5] + in.z * m[ 9] + m[13],
              in.x * m[ 2] + in.y * m[ 6] + in.z * m[10] + m[14]);
}

