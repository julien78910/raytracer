#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "data.h"
#include "vector3.h"
#include "raytracer.h"
#include "visual.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  s_scene *scene = parse(argv[1]);

  s_color **output = ray_tracer(scene);

  display(argv[2], output, scene->screen);

  return 0;
}

s_color **ray_tracer(s_scene *scene)
{
  s_camera cam = scene->camera;
  s_screen screen = scene->screen;

  s_vec3 vec_u = normalize(cam.u);
  s_vec3 vec_v = normalize(cam.v);
  s_vec3 vec_w = cross_prod(vec_u, vec_v);

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

      s_color color = pixel_color(scene, point);
      output[w + screen.width / 2][h + screen.height / 2] = color;
    }
  }

  return output;
}


s_color pixel_color(s_scene *scene, s_vec3 pixel)
{
  s_vec3 dir = compute(scene->camera.pos, pixel);
  s_sphere *sphere = scene->sphere;
  s_plane *plane = scene->plane;
  s_triangle *triangle = scene->triangle;

  s_sphere *sp = scene->sphere;
  s_plane *pl = scene->plane;
  s_triangle *tr = scene->triangle;

  s_vec3 clos_sphere = sphere_intersec(dir, scene->camera.pos, pixel, sphere);
  s_vec3 clos_plane = plane_intersec(dir, scene->camera.pos, plane);
  s_vec3 clos_triangle = triangle_intersec(dir, scene->camera.pos, triangle);

  while (sp != NULL)
  {
    s_vec3 intersec = sphere_intersec(dir, scene->camera.pos, pixel, sp);

    if (!is_equal(intersec, scene->camera.pos)
        && distance(intersec, pixel) < distance(clos_sphere, pixel))
    {
      sphere = sp;
      clos_sphere = intersec;
    }
    sp = sp->next;
  }

  while (pl != NULL)
  {
    s_vec3 intersec = plane_intersec(dir, scene->camera.pos, pl);

    if (!is_equal(intersec, scene->camera.pos)
        && distance(intersec, pixel) < distance(clos_plane, pixel))
    {
      plane = pl;
      clos_plane = intersec;
    }
    pl = pl->next;
  }

  while (tr != NULL)
  {
    s_vec3 intersec = triangle_intersec(dir, scene->camera.pos, tr);

    if (!is_equal(intersec, scene->camera.pos)
        && distance(intersec, pixel) < distance(clos_triangle, pixel))
    {
      triangle = tr;
      clos_triangle = intersec;
    }
    tr = tr->next;
  }

  float dist_triangle = distance(clos_triangle, pixel);
  float dist_plane = distance(clos_plane, pixel);
  float dist_sphere = distance(clos_sphere, pixel);

  s_spe spe;
  s_vec3 normal;
  s_vec3 closest_inter;

  if (dist_sphere < dist_triangle && dist_sphere < dist_plane)
  {
    spe = sphere->spe;
    closest_inter = clos_sphere;
    normal = compute(sphere->pos, clos_sphere);
  }
  else if (dist_plane < dist_sphere && dist_plane < dist_triangle)
  {
    spe = plane->spe;
    closest_inter = clos_plane;
    normal.x = plane->a;
    normal.y = plane->b;
    normal.z = plane->c;

  }
  else
  {
    spe = triangle->spe;
    closest_inter = clos_triangle;
    s_vec3 vec_ab = compute(triangle->a, triangle->b);
    s_vec3 vec_ac = compute(triangle->a, triangle->c);
    normal = cross_prod(vec_ab, vec_ac);
  }


  s_color dir_color = dir_light(scene->dlight, spe, normal);
  s_color point_color = point_light(scene->plight, spe, closest_inter, normal);
  s_color amb_color = ambient_light(scene->alight, spe);

  s_color color;
  color.r = amb_color.r + dir_color.r + point_color.r;
  color.g = amb_color.g + dir_color.g + point_color.g;
  color.b = amb_color.b + dir_color.b + point_color.b;

  return color;
}


s_vec3 triangle_intersec(s_vec3 dir, s_vec3 ray_pos, s_triangle *triangle)
{
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

  s_vec3 result = plane_intersec(dir, ray_pos, plane);
  float s = vec_ab.x * result.y - triangle->a.y * vec_ab.x
    - result.x * vec_ab.y + vec_ab.y * triangle->a.x;
  s /= (vec_ac.y * vec_ab.x - vec_ac.x * vec_ab.y);

  float r = (result.x - triangle->a.x - s * vec_ac.x) / vec_ab.x;

  if (s >= 0 && s <= 1 && r >= 0 && r <= 1 && s + r >= 0 && s + r <= 1)
    return result;

  return ray_pos;
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

  if (distance(p0, intersec) > distance(p1, intersec))
    result = p1;
  
  return result;
}


s_color ambient_light(s_alight *alight, s_spe spe)
{
  s_color amb_color;

  amb_color.r = 0;
  amb_color.g = 0;
  amb_color.b = 0;

  for (; alight; alight = alight->next)
  {
    amb_color.r += ((alight->color.r / 255) * spe.color.r);
    amb_color.g += ((alight->color.g / 255) * spe.color.g);
    amb_color.b += ((alight->color.b / 255) * spe.color.b);
  }

  return amb_color;
}


s_color dir_light(s_dlight *dlight, s_spe spe, s_vec3 normal)
{
  s_color color;

  color.r = 0;
  color.g = 0;
  color.b = 0;

  for (; dlight; dlight = dlight->next)
  {
    float ld = dot_prod(normalize(scale(dlight->dir, -1)), normal) * spe.diff;

    if (ld < 0)
      ld = 0;

    color.r += ((dlight->color.r / 255) * spe.color.r * ld);
    color.g += ((dlight->color.g / 255) * spe.color.g * ld);
    color.b += ((dlight->color.b / 255) * spe.color.b * ld);
  }

  return color;
}


s_color point_light(s_plight *plight, s_spe spe, s_vec3 point, s_vec3 normal)
{
  s_color color;

  color.r = 0;
  color.g = 0;
  color.b = 0;

  for (; plight; plight = plight->next)
  {
    s_vec3 vec_light_obj = compute(point, plight->pos);
    float ld = dot_prod(normalize(vec_light_obj), normal) * spe.diff;
    float atten = 1 / distance(point, plight->pos);
    ld *= atten;
 
    if (ld < 0)
      ld = 0;


    color.r += ((plight->color.r / 255) * spe.color.r * ld);
    color.g += ((plight->color.g / 255) * spe.color.g * ld);
    color.b += ((plight->color.b / 255) * spe.color.b * ld);
  }

  return color;
}
