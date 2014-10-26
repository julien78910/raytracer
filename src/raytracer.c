#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "data.h"
#include "vector3.h"
#include "visual.h"
#include "destroyer.h"
#include "lighting.h"
#include "intersec.h"
#include "raytracer.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  s_scene *scene = parse(argv[1]);

  s_color **output = ray_tracer(scene);

  display(argv[2], output, scene->screen);

  destroy_all(scene, output);

  return 0;
}

static s_color **ray_tracer(s_scene *scene)
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


static s_sphere *closest_sphere(s_scene *scene, s_vec3 pixel,
    s_vec3 dir, s_vec3 **clos_sphere)
{
  s_sphere *sp = scene->sphere;
  s_sphere *sphere = NULL;

  while (sp != NULL)
  {
    s_vec3 *intersec = sphere_intersec(dir, scene->camera.pos, pixel, sp);

    if (intersec && (!*clos_sphere
        || distance(*intersec, pixel) < distance(**clos_sphere, pixel)))
    {
      sphere = sp;
      *clos_sphere = intersec;
    }
    sp = sp->next;
  }

  return sphere;
}


static s_plane *closest_plane(s_scene *scene, s_vec3 pixel,
    s_vec3 dir, s_vec3 **clos_plane)
{
  s_plane *plane = NULL;
  s_plane *pl = scene->plane;

  while (pl != NULL)
  {
    s_vec3 *intersec = plane_intersec(dir, scene->camera.pos, pl);

    if (intersec && (!*clos_plane
          || distance(*intersec, pixel) < distance(**clos_plane, pixel)))
    {
      plane = pl;
      *clos_plane = intersec;
    }
    pl = pl->next;
  }

  return plane;
}

static s_color get_obj(s_scene *scene, s_v9obj o, s_vec3 dist, s_color color)
{
  s_spe spe;
  s_vec3 normal;
  s_vec3 closest_inter;

  if (o.obj.sphere != NULL && dist.x < dist.z && dist.x < dist.y)
  {
    spe = o.obj.sphere->spe;
    closest_inter = *o.clos_obj.sphere;
    normal = compute(o.obj.sphere->pos, *o.clos_obj.sphere);
  }

  else if (o.obj.plane != NULL
      && dist.y < dist.x && dist.y < dist.z)
  {
    spe = o.obj.plane->spe;
    closest_inter = *o.clos_obj.plane;
    normal.x = o.obj.plane->a;
    normal.y = o.obj.plane->b;
    normal.z = o.obj.plane->c;

  }
  else if (o.obj.triangle != NULL)
  {
    spe = o.obj.triangle->spe;
    closest_inter = *o.clos_obj.triangle;
    s_vec3 vec_ab = compute(o.obj.triangle->a, o.obj.triangle->b);
    s_vec3 vec_ac = compute(o.obj.triangle->a, o.obj.triangle->c);
    normal = cross_prod(vec_ab, vec_ac);
  }
  else
  {
    return color;
  }

  normal = normalize(normal);

  color = set_color(scene, spe, normal, closest_inter);
  return color;
}


static s_triangle *closest_triangle(s_scene *scene, s_vec3 pixel,
    s_vec3 dir, s_vec3 **clos_triangle)
{
  s_triangle *triangle = NULL;
  s_triangle *tr = scene->triangle;

  while (tr != NULL)
  {
    s_vec3 *intersec = triangle_intersec(dir, scene->camera.pos, tr);

    if (intersec && (!*clos_triangle
        || distance(*intersec, pixel) < distance(**clos_triangle, pixel)))
    {
      triangle = tr;
      *clos_triangle = intersec;
    }
    tr = tr->next;
  }

  return triangle;
}

s_color colorize_pixel(s_scene *scene, s_vec9 clos_obj, s_v3obj obj)
{
  s_vec3 *clos_triangle = clos_obj.triangle;
  s_vec3 *clos_sphere = clos_obj.sphere;
  s_vec3 *clos_plane = clos_obj.plane;


  float dist_triangle = INFINITY;
  if (clos_triangle)
    dist_triangle = distance(*clos_triangle, scene->camera.pos);
  float dist_plane = INFINITY;
  if (clos_plane)
    dist_plane = distance(*clos_plane, scene->camera.pos);
  float dist_sphere = INFINITY;
  if (clos_sphere)
    dist_sphere = distance(*clos_sphere, scene->camera.pos);

  s_vec3 dist;
  dist.x = dist_sphere;
  dist.y = dist_plane;
  dist.z = dist_triangle;
  s_color color;
  color.r = 0;
  color.g = 0;
  color.b = 0;
  s_v9obj o;
  o.clos_obj = clos_obj;
  o.obj = obj;
  return get_obj(scene, o, dist, color);
}


static s_color pixel_color(s_scene *scene, s_vec3 pixel)
{
  s_vec3 dir = compute(scene->camera.pos, pixel);

  s_vec3 *clos_sphere = NULL;
  s_sphere *sphere = closest_sphere(scene, pixel, dir, &clos_sphere);

  s_vec3 *clos_plane = NULL;
  s_plane *plane = closest_plane(scene, pixel, dir, &clos_plane);

  s_vec3 *clos_triangle = NULL;
  s_triangle *triangle = closest_triangle(scene, pixel, dir, &clos_triangle);

  s_v3obj obj;
  obj.sphere = sphere;
  obj.plane = plane;
  obj.triangle = triangle;
  s_vec9 clos_obj;
  clos_obj.sphere = clos_sphere;
  clos_obj.plane = clos_plane;
  clos_obj.triangle = clos_triangle;
  s_color color = colorize_pixel(scene, clos_obj, obj);
  free(clos_triangle);
  free(clos_sphere);
  free(clos_plane);
  return color;
}


static s_color set_color(s_scene *sc, s_spe spe, s_vec3 nor, s_vec3 clos_inter)
{
  s_color color;

  s_color dir_color = dir_light(sc->dlight, spe, nor);
  s_color point_color = point_light(sc->plight, spe, clos_inter, nor);
  s_color amb_color = ambient_light(sc->alight, spe);

  s_color spec_light = specular_lighting(nor, sc, clos_inter, spe);


  color.r = dir_color.r + point_color.r + amb_color.r + spec_light.r;
  color.g = dir_color.g + point_color.g + amb_color.g + spec_light.g;
  color.b = dir_color.b + point_color.b + amb_color.b + spec_light.b;

  return color;
}
