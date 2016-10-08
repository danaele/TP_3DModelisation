#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>

struct vec2
{
  float x, y;
  vec2(float x=0,float y=0):x(x),y(y){}
};

struct vec3
{
  float x, y, z;
  vec3(float x=0,float y=0,float z=0):x(x),y(y),z(z){}
  vec3 operator+(const vec3&v)const{return vec3(x+v.x, y+v.y, z+v.z);}
  vec3 operator-(const vec3&v)const{return vec3(x-v.x, y-v.y, z-v.z);}
  vec3 operator*(const vec3&v)const{return vec3(x*v.x, y*v.y, z*v.z);}
  vec3 operator/(const vec3&v)const{return vec3(x/v.x, y/v.y, z/v.z);}

  vec3& operator+=(const vec3&v){x+=v.x;y+=v.y;z+=v.z;return*this;}
  vec3& operator-=(const vec3&v){x-=v.x;y-=v.y;z-=v.z;return*this;}
  vec3& operator*=(const vec3&v){x*=v.x;y*=v.y;z*=v.z;return*this;}
  vec3& operator/=(const vec3&v){x/=v.x;y/=v.y;z/=v.z;return*this;}
  float dot (const vec3&v)const{return x*v.x+y*v.y+z*v.z;}
  float length (void)const{return std::sqrt(dot(*this));}
  vec3& normalize (void){float l=length();x/=l;y/=l;z/=l;return*this;}

  vec3 cross(const vec3&v)const{return vec3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);}

};

struct matrix
{
  float m[16];
  matrix();
  void translate(const vec3 &trans);
  void scale(float factor);
  void scale(const vec3 &factor);
  void rotate(float angle, const vec3 &v);
  void set_perspective(float fovy, float aspect, float near, float far);
  void set_ortho(float left, float right, float bottom, float top, float near, float far);
  void set_look_at(const vec3 &eye, const vec3 &center, const vec3 &up);
  matrix operator*(const matrix &mat) const;
};

#endif
