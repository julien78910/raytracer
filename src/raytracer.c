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
    {
      if (output[i][j].g == 255)
      {
        printf("\033[32m");
        printf("o");
        printf("\033[0m");
      }

      else
        printf("o");
    }
    printf("\n");
  }

}

s_color **ray_tracer(s_scene *scene)
{
  s_camera cam = scene->camera;
  s_screen screen = scene->screen;

  s_vec3 vec_u = normalize(cam.u);
  s_vec3 vec_v = normalize(cam.v);
  s_vec3 vec_w = cross_prod(vec_u, vec_v);

  printf("u = (%f, %f, %f)\nv = (%f, %f, %f)\n", vec_u.x, vec_u.y, vec_u.z, vec_v.x, vec_v.y, vec_v.z);
  printf("w = (%f, %f, %f)\n", vec_w.x, vec_w.y, vec_w.z);

  printf("width = %d\n", screen.width);

  float dist_l = (screen.width / 2) / tan(45.f / 2.f);
  printf("L = %f\n", dist_l);
  s_vec3 center = add(cam.pos, scale(vec_w, dist_l));

  printf("center = (%f, %f, %f)\n", center.x, center.y, center.z);

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
      s_vec3 intersec = find_intersec(dir, scene, point);
      

      s_color color;
      color.r = 0;
      color.g = 0;
      color.b = 0;

      //printf("%d: inter (%f, %f, %f)\n", w + screen.width / 2, intersec.x, intersec.y, intersec.z);
      //printf("cam (%f, %f, %f)\n", cam.pos.x, cam.pos.y, cam.pos.z);

      if (intersec.x != cam.pos.x || intersec.y != cam.pos.y
          || intersec.z != cam.pos.z)
      {
        color.r = 0;
        color.g = 255;
        color.b = 0;
      }
      
      output[w + screen.width / 2][h + screen.height / 2] = color;
    }
  }

  return output;
}



s_vec3 find_intersec(s_vec3 dir, s_scene *scene, s_vec3 point)
{
  s_sphere obj = *(scene->sphere);
  s_vec3 cam = scene->camera.pos;

  float a = dot_prod(dir, dir);
  float b = dot_prod(scale(dir, 2), add(cam, scale(obj.pos, -1)));
  float c = pow(distance(cam, scale(obj.pos, -1)), 2) - pow(obj.radius, 2);

  float delta = b * b - 4 * a * c;

  if (delta < 0)
    return cam;

  float t0 = (-b - sqrt(delta)) / (2 * a);
  float t1 = (-b + sqrt(delta)) / (2 * a);
  
  s_vec3 p0 = add(cam, scale(dir, t0));
  s_vec3 p1 = add(cam, scale(dir, t1));

  s_vec3 result = p0;
  s_vec3 aux = p1;

  printf("delta = %f, t0 = %f, t1 = %f\n", delta, t0, t1);
  printf("p0 = (%f, %f, %f)\np1 = (%f, %f, %f)\n", p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);

  if (distance(p0, point) > distance(p1, point))
  {
    result = p1;
    aux = p0;
  }
  
  if (dot_prod(point, result) < 0)
  {
    result = aux;
    if (dot_prod(point, result) < 0)
      return cam;
  }

  return result;
}
