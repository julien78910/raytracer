#ifndef DATA_H
# define DATA_H

# include <stdio.h>
# include <stdlib.h>
# include "vector3.h"

struct screen
{
  size_t width;
  size_t height;
} typedef s_screen;

struct camera
{
  s_vec3 pos;
  s_vec3 u;
  s_vec3 v;
} typedef s_camera;

struct sphere
{
  float radius;
  s_vec3 pos;
  float  diff;
  float  refl;
  float  spec;
  float  shin;
  float  refr;
  float  opac;
  float color_r;
  float color_g;
  float color_b;
  struct sphere *next;
} typedef s_sphere;

struct plane
{
  float a;
  float b;
  float c;
  float d;
  float diff;
  float refl;
  float spec;
  float shin;
  float refr;
  float opac;
  float color_r;
  float color_g;
  float color_b;
  struct plane *next;
} typedef s_plane;

struct triangle
{
  s_vec3 a;
  s_vec3 b;
  s_vec3 c;
  float diff;
  float refl;
  float spec;
  float shin;
  float refr;
  float opac;
  float color_r;
  float color_g;
  float color_b;
  struct triangle *next;
} typedef s_triangle;

struct plight
{
  s_vec3 pos;
  float color_r;
  float color_g;
  float color_b;
  struct plight *next;
} typedef s_plight;

struct dlight
{
  s_vec3 dir;
  float color_r;
  float color_g;
  float color_b;
  struct dlight *next;
} typedef s_dlight;

struct alight
{
  float color_r;
  float color_g;
  float color_b;
  struct dlight *next;
} typedef s_alight;

struct scene
{
  s_screen screen;
  s_camera camera;
  s_sphere *sphere;
  s_plane *plane;
  s_triangle *triangle;
  s_plight *plight;
  s_dlight *dlight;
  s_alight *alight;
} typedef s_scene;

#endif /* !DATA_H */
