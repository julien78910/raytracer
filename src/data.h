#ifndef DATA_H
# define DATA_H

# include <stdio.h>
# include <stdlib.h>
# include "vector3.h"

typedef struct color
{
  float r;
  float g;
  float b;
} s_color;

typedef struct screen
{
  int width;
  int height;
} s_screen;

typedef struct camera
{
  s_vec3 pos;
  s_vec3 u;
  s_vec3 v;
} s_camera;

typedef struct sphere
{
  float radius;
  s_vec3 pos;
  float  diff;
  float  refl;
  float  spec;
  float  shin;
  float  refr;
  float  opac;
  s_color color;
  struct sphere *next;
} s_sphere;

typedef struct plane
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
  s_color color;
  struct plane *next;
} s_plane;

typedef struct triangle
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
  s_color color;
  struct triangle *next;
} s_triangle;

typedef struct plight
{
  s_vec3 pos;
  s_color color;
  struct plight *next;
} s_plight;

typedef struct dlight
{
  s_vec3 dir;
  s_color color;
  struct dlight *next;
} s_dlight;

typedef struct alight
{
  s_color color;
  struct alight *next;
} s_alight;

typedef struct scene
{
  s_screen screen;
  s_camera camera;
  s_sphere *sphere;
  s_plane *plane;
  s_triangle *triangle;
  s_plight *plight;
  s_dlight *dlight;
  s_alight *alight;
} s_scene;

#endif /* !DATA_H */
