#include <stdlib.h>
#include "math.h"
#include "vector3.h"
#include "data.h"
#include "lighting.h"


static s_color specular_dlight(s_vec3 nor, s_scene *sc, s_vec3 pix, s_spe spe)
{
  s_dlight *dlight = sc->dlight;

  s_color result;
  result.r = 0;
  result.g = 0;
  result.b = 0;

  s_vec3 to_cam = normalize(compute(pix, sc->camera.pos));

  for (; dlight; dlight = dlight->next)
  {
    s_vec3 from_light = normalize(scale(dlight->dir, 1.f));
    s_vec3 reflect = scale(nor, 2.f * dot_prod(from_light, nor));
    reflect = add(from_light, scale(reflect, -1.f));
    float cos_angle = dot_prod(to_cam, reflect);
    if (cos_angle < 0)
      continue;
    float ls = pow(cos_angle, spe.shin);
    ls *= spe.spec;

    result.r = ls * dlight->color.r;
    result.g = ls * dlight->color.g;
    result.b = ls * dlight->color.b;
  }

  return result;
}


static s_color specular_plight(s_vec3 nor, s_scene *sc, s_vec3 pix, s_spe spe)
{
  s_plight *plight = sc->plight;

  s_color result;
  result.r = 0;
  result.g = 0;
  result.b = 0;

  s_vec3 to_cam = normalize(compute(pix, sc->camera.pos));

  for (; plight; plight = plight->next)
  {
    s_vec3 from_light = normalize(scale(plight->pos, 1.f));
    s_vec3 reflect = scale(nor, 2.f * dot_prod(from_light, nor));
    reflect = add(from_light, scale(reflect, -1.f));
    float cos_angle = dot_prod(to_cam, reflect);
    if (cos_angle < 0)
      continue;
    float ls = pow(cos_angle, spe.shin);
    ls *= spe.spec;

    result.r = ls * plight->color.r;
    result.g = ls * plight->color.g;
    result.b = ls * plight->color.b;
  }

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
    amb_color.r += ((alight->color.r / 255.f) * spe.color.r);
    amb_color.g += ((alight->color.g / 255.f) * spe.color.g);
    amb_color.b += ((alight->color.b / 255.f) * spe.color.b);
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

    color.r += ((dlight->color.r / 255.f) * spe.color.r * ld);
    color.g += ((dlight->color.g / 255.f) * spe.color.g * ld);
    color.b += ((dlight->color.b / 255.f) * spe.color.b * ld);
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
    float ld = dot_prod(normal, normalize(vec_light_obj)) * spe.diff;
    float atten = 1 / distance(point, plight->pos);
    ld *= atten;
 
    if (ld < 0)
      ld = 0;


    color.r += ((plight->color.r / 255.f) * spe.color.r * ld);
    color.g += ((plight->color.g / 255.f) * spe.color.g * ld);
    color.b += ((plight->color.b / 255.f) * spe.color.b * ld);
  }

  return color;
}


s_color specular_lighting(s_vec3 norm, s_scene *sc, s_vec3 pixel, s_spe spe)
{
  s_color result = specular_dlight(norm, sc, pixel, spe);
  s_color aux = specular_plight(norm, sc, pixel, spe);
  
  result.r += aux.r;
  result.g += aux.g;
  result.b += aux.b;

  return result;
}
