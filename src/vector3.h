#ifndef VECTOR3_H
# define VECTOR3_H

# include <stdio.h>
# include <stdlib.h>

typedef struct vector3
{
  float x;
  float y;
  float z;
} s_vec3;

s_vec3 add(s_vec3 v1, s_vec3 v2);
s_vec3 scale(s_vec3 v, float c);
float dot_prod(s_vec3 v1, s_vec3 v2);
s_vec3 cross_prod(s_vec3 v1, s_vec3 v2);
s_vec3 normalize(s_vec3 v);
float distance(s_vec3 v1, s_vec3 v2);
s_vec3 compute(s_vec3 v1, s_vec3 v2);


#endif /* !VECTOR3_H */
