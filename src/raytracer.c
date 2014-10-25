#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "data.h"
#include "vector3.h"
#include "raytracer.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  s_scene *scene = parse(argv[1]);

  ray_tracer(scene);

}

s_color *ray_tracer(s_scene *scene)
{
  s_camera cam = scene->camera;
  s_screen screen = scene->screen;

  s_vec3 vec_u = normalize(cam.u);
  s_vec3 vec_v = normalize(cam.v);
  s_vec3 vec_w = cross_prod(vec_u, vec_v);

  float dist_l = (screen.width / 2) / tan(45 / 2);
  s_vec3 center = add(cam.pos, scale(vec_w, dist_l));

  s_color *output = malloc(screen.height * screen.width * sizeof (s_color));

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

      if (intersec.x != cam.pos.x && intersec.y != cam.pos.y
          && intersec.z != cam.pos.z)
      {
        color.r = 0;
        color.g = 255;
        color.b = 0;
      }
      
      output[w] = color;
    }
  }

  return output;
}



s_vec3 find_intersec(s_vec3 dir, s_scene *scene, s_vec3 point)
{
  s_sphere obj;
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
