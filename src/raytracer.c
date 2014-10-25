#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "data.h"
#include "vector3.h"
#include "raytracer.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  s_scene *scene = parse(argv[1]);

  s_color **output = ray_tracer(scene);

  for (int i = 0; i < scene->screen.width; i++)
  {
    for (int j = 0; j < scene->screen.height; j++)
      printf("%d %d %d  ", (int)output[i][j].r, (int)output[i][j].g, (int)output[i][j].b);
    printf("\n");
  }
  if (output)
    return 1;

  return 0;
}

s_color **ray_tracer(s_scene *scene)
{
  s_camera cam = scene->camera;
  s_screen screen = scene->screen;

  s_vec3 vec_u = normalize(cam.u);
  s_vec3 vec_v = normalize(cam.v);
  s_vec3 vec_w = cross_prod(vec_u, vec_v);

  printf("P3\n");
  printf("%d %d\n255\n", screen.height, screen.width);

  float dist_l = (screen.width / 2) / tan(45.f / 2.f);
  s_vec3 center = add(cam.pos, scale(vec_w, dist_l));

  s_color **output = malloc(screen.width * sizeof (s_color *));

  for (int i = 0; i < screen.width; i++)
    output[i] = malloc(screen.height * sizeof (s_color));

  for (int w = -screen.width / 2; w < screen.width / 2; w++)
  {
    for (int h = -screen.height / 2; h < screen.height / 2; h++)
    {
      s_vec3 point;
      point.x = center.x + w * (vec_u.x + vec_v.x);
      point.y = center.y + h * (vec_u.y + vec_v.y);
      point.z = center.z;

      s_vec3 dir = compute(cam.pos, point);

      s_sphere *aux = scene->sphere;
      s_sphere *closest = aux;
      s_vec3 closest_inter = sphere_intersec(dir, scene->camera.pos, point, aux);
      s_color color;
      color.r = 0;
      color.g = 0;
      color.b = 0;

      while (aux != NULL)
      {
        s_vec3 intersec = sphere_intersec(dir, scene->camera.pos, point, aux);
     
        if (distance(intersec, point) < distance(closest_inter, point))
        {
          closest = aux;
          closest_inter = intersec;
        }
        aux = aux->next;
      }

      if (closest_inter.x != cam.pos.x || closest_inter.y != cam.pos.y
          || closest_inter.z != cam.pos.z)
      {
        s_color dir_color = dir_light(scene, closest, closest_inter);
        s_color point_color = point_light(scene, closest, closest_inter);
        s_color amb_color = ambient_light(scene, closest);

        color.r = amb_color.r + dir_color.r + point_color.r;
        color.g = amb_color.g + dir_color.g + point_color.g;
        color.b = amb_color.b + dir_color.b + point_color.b;
      }

      output[w + screen.width / 2][h + screen.height / 2] = color;

    }
  }

  return output;
}

s_vec3 plane_intersec(s_vec3 dir, s_vec3 ray_pos, s_plane *plane)
{
  float denum = plane->a * dir.x + plane->b * dir.x + plane->c * dir.x;

  if (denum == 0)
    return ray_pos;

  float num = plane->a * ray_pos.x + plane->b * ray_pos.y
    + plane->c * ray_pos.z + plane->d;

  float t0 = -(num / denum);

  if (t0 > 0)
    return add(ray_pos, scale(dir, t0));

  return ray_pos;
}


s_vec3 sphere_intersec(s_vec3 dir, s_vec3 ray_pos,
    s_vec3 intersec, s_sphere *sphere)
{
  float a = dot_prod(dir, dir);
  float b = dot_prod(scale(dir, 2), add(ray_pos, scale(sphere->pos, -1)));
  float c = pow(distance(ray_pos,
        scale(sphere->pos, -1)), 2) - pow(sphere->radius, 2);

  float delta = b * b - 4 * a * c;

  if (delta < 0)
    return ray_pos;

  float t0 = (-b - sqrt(delta)) / (2 * a);
  float t1 = (-b + sqrt(delta)) / (2 * a);
  
  s_vec3 p0 = add(ray_pos, scale(dir, t0));
  s_vec3 p1 = add(ray_pos, scale(dir, t1));

  s_vec3 result = p0;
  s_vec3 aux = p1;

  if (distance(p0, intersec) > distance(p1, intersec))
  {
    result = p1;
    aux = p0;
  }
  
  if (dot_prod(intersec, result) < 0)
  {
    result = aux;
    if (dot_prod(intersec, result) < 0)
      return ray_pos;
  }

  return result;
}

s_color ambient_light(s_scene *scene, s_sphere *closest)
{
  s_alight *alight = scene->alight;
  s_color amb_color;

  if (alight == NULL)
  {
    amb_color.r = 0;
    amb_color.g = 0;
    amb_color.b = 0;

    return amb_color;
  }

  amb_color.r = (scene->alight->color.r / 255) * closest->color.r;
  amb_color.g = (scene->alight->color.g / 255) * closest->color.g;
  amb_color.b = (scene->alight->color.b / 255) * closest->color.b;

  return amb_color;
}

s_color dir_light(s_scene *scene, s_sphere *obj, s_vec3 point)
{
  s_dlight *dlight = scene->dlight;
  s_color color;

  if (dlight == NULL)
  {
    color.r = 0;
    color.g = 0;
    color.b = 0;
    return color;
  }

  s_vec3 normal = normalize(compute(obj->pos, point));
  float ld = dot_prod(normalize(scale(dlight->dir, -1)), normal) * obj->diff;
 
  if (ld < 0)
    ld = 0;

  color.r = (dlight->color.r / 255) * obj->color.r * ld;
  color.g = (dlight->color.g / 255) * obj->color.g * ld;
  color.b = (dlight->color.b / 255) * obj->color.b * ld;

  return color;
}


s_color point_light(s_scene *scene, s_sphere *obj, s_vec3 point)
{
  s_plight *plight = scene->plight;
  s_color color;

  if (plight == NULL)
  {
    color.r = 0;
    color.g = 0;
    color.b = 0;
    return color;
  }

  s_vec3 normal = normalize(compute(obj->pos, point));
  s_vec3 vec_light_obj = compute(point, plight->pos);
  float ld = dot_prod(normalize(vec_light_obj), normal) * obj->diff;
  float atten = 1 / distance(point, plight->pos);
  ld *= atten;
 
  if (ld < 0)
    ld = 0;


  color.r = (plight->color.r / 255) * obj->color.r * ld;
  color.g = (plight->color.g / 255) * obj->color.g * ld;
  color.b = (plight->color.b / 255) * obj->color.b * ld;

  return color;
}
