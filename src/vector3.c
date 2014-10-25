#include <stdio.h>
#include "vector3.h"

/**
** Add 2 vector3 together.
*/
s_vec3 add(s_vec3 v1, s_vec3 v2)
{
  s_vec3 new;
  new.x = v1.x + v2.x;
  new.y = v1.y + v2.y;
  new.z = v1.z + v2.z;
  return new;
}

/**
** Get scale of the vector3 with a precise float.
*/
s_vec3 scale(s_vec3 v, float c)
{
  s_vec3 new;
  new.x = v.x * c;
  new.y = v.y * c;
  new.z = v.z * c;
  return new;
}

/**
** Return the product between two points.
*/
float dot_prod(s_vec3 v1, s_vec3 v2)
{
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

/**
** Return the cross product between two vector3.
*/
s_vec3 cross_prod(s_vec3 v1, s_vec3 v2)
{
  s_vec3 new;
  new.x = (v1.y * v2.z) - (v1.z * v2.y);
  new.y = (v1.z * v2.x) - (v1.x * v2.z);
  new.z = (v1.x * v2.y) - (v1.y * v2.x);
  return new;
}
