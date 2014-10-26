#include <stdlib.h>
#include "math.h"
#include "vector3.h"
#include "data.h"


s_vec3 *sphere_intersec(s_vec3 dir, s_vec3 ray_pos,
    s_vec3 intersec, s_sphere *sphere)
{
  if (sphere == NULL)
    return NULL;

  float a = dot_prod(dir, dir);
  float b = dot_prod(scale(dir, 2), add(ray_pos, scale(sphere->pos, -1)));
  float c = pow(distance(ray_pos,
        scale(sphere->pos, -1)), 2) - pow(sphere->radius, 2);

  float delta = b * b - 4 * a * c;

  if (delta < 0)
    return NULL;

  float t0 = (-b - sqrt(delta)) / (2 * a);
  float t1 = (-b + sqrt(delta)) / (2 * a);
  
  s_vec3 p0 = add(ray_pos, scale(dir, t0));
  s_vec3 p1 = add(ray_pos, scale(dir, t1));

  if (distance(p0, intersec) > distance(p1, intersec))
    p1 = p0;

  s_vec3 *result = malloc(sizeof (s_vec3));
  result->x = p0.x;
  result->y = p0.y;
  result->z = p0.z;
  
  return result;
}



s_vec3 *plane_intersec(s_vec3 dir, s_vec3 ray_pos, s_plane *plane)
{
  if (plane == NULL)
    return NULL;

  float denum = plane->a * dir.x + plane->b * dir.y + plane->c * dir.z;

  if (denum == 0)
    return NULL;

  float num = plane->a * ray_pos.x + plane->b * ray_pos.y
    + plane->c * ray_pos.z + plane->d;

  float t0 = -(num / denum);

  if (t0 < 0)
    return NULL;
  
  s_vec3 *result = malloc(sizeof (s_vec3));
  s_vec3 aux = add(ray_pos, scale(dir, t0));

  result->x = aux.x;
  result->y = aux.y;
  result->z = aux.z;

  return result;
}



s_vec3 *triangle_intersec(s_vec3 dir, s_vec3 ray_pos, s_triangle *triangle)
{
  if (triangle == NULL)
    return NULL;

  s_vec3 vec_ab = compute(triangle->a, triangle->b);
  s_vec3 vec_ac = compute(triangle->a, triangle->c);

  s_vec3 normal = cross_prod(vec_ab, vec_ac);

  float d = -triangle->a.x * normal.x - triangle->a.y * normal.y
    - triangle->a.z * normal.z;

  s_plane *plane = malloc(sizeof (s_plane));
  plane->a = normal.x;
  plane->b = normal.y;
  plane->c = normal.z;
  plane->d = d;
  plane->spe = triangle->spe;
  plane->next = NULL;

  s_vec3 *result = plane_intersec(dir, ray_pos, plane);
 
  if (result == NULL)
    return NULL;

  float s = vec_ab.x * result->y - triangle->a.y * vec_ab.x
    - result->x * vec_ab.y + vec_ab.y * triangle->a.x;
  s /= (vec_ac.y * vec_ab.x - vec_ac.x * vec_ab.y);

  float r = (result->x - triangle->a.x - s * vec_ac.x) / vec_ab.x;

  if (s >= 0 && s <= 1 && r >= 0 && r <= 1 && s + r >= 0 && s + r <= 1)
    return result;

  return NULL;
}
