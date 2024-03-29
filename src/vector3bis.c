#include <math.h>
#include <stdlib.h>
#include "vector3.h"

/**
**  Normalize the vector put in argument.
*/
s_vec3 normalize(s_vec3 v)
{
  s_vec3 new;
  new.x = v.x / (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
  new.y = v.y / (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
  new.z = v.z / (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
  return new;
}

/**
** Return the distance between 2 points of the space.
*/
float distance(s_vec3 d1, s_vec3 d2)
{
  return sqrt(pow(d1.x - d2.x, 2) + pow(d1.y - d2.y, 2) + pow(d1.z - d2.z, 2));
}

/**
**  Compute 2 points in the space into a single vector3.
*/
s_vec3 compute(s_vec3 d1, s_vec3 d2)
{
  s_vec3 new;
  new.x = d2.x - d1.x;
  new.y = d2.y - d1.y;
  new.z = d2.z - d1.z;
  return new;
}

/**
** Return 1 if the two vectors3 are equals. 0 if they are not.
*/
int is_equal(s_vec3 v1, s_vec3 v2)
{
  if ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z))
    return 1;
  else
    return 0;
}
