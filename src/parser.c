#define _BSD_SOURCE
#include "parser.h"
#include "data.h"
#include "vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
** Fill the structure screen contained in the line saveptr.
*/
static s_screen fill_screen(char **saveptr)
{
  char *bf = strtok_r(NULL, " ", saveptr);
  s_screen scr;
  scr.width = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  scr.height = strtof(bf, NULL);
  return scr;
}

/**
** Fill the structure camera contained in the line saveptr.
*/
static s_camera fill_camera(char **saveptr)
{
  char *bf = strtok_r(NULL, " ", saveptr);
  s_camera came;
  came.pos.x = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.pos.y = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.pos.z = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.u.x = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.u.y = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.u.z = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.v.x = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.v.y = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  came.v.z = strtof(bf, NULL);
  return came;
}

s_spe fill_spe(char **saveptr)
{
  s_spe spe;
  char *bf = strtok_r(NULL, " ", saveptr);
  spe.diff =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  spe.refl =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  spe.spec =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  spe.shin =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  spe.refr =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  spe.opac =  strtof(bf, NULL);
  s_color col;
  bf = strtok_r(NULL, " ", saveptr);
  col.r =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.g =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.b =  strtof(bf, NULL);
  spe.color = col;
  return spe;
}

/**
** Fill a new struct sphere which will be at the last position in the list
** of spheres.
*/
s_sphere *fill_sphere(s_scene *sc, char **saveptr)
{
  s_sphere *sp = sc->sphere;
  char *bf = strtok_r(NULL, " ", saveptr);
  if (sc->sphere)
  {
    while (sp->next)
      sp = sp->next;
  }
  s_sphere *new = malloc(sizeof (s_sphere));
  new->radius = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->pos.x =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->pos.y =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->pos.z =  strtof(bf, NULL);
  new->spe = fill_spe(saveptr);
  new->next = NULL;
  if (sp)
    sp->next = new;
  else
    return new;
  return sc->sphere;
}

/**
** Fill a new struct triangle which will be at the last position in the list
** of triangles.
*/
s_triangle *fill_triangle(s_scene *sc, char **saveptr)
{
  s_triangle *sp = sc->triangle;
  char *bf = strtok_r(NULL, " ", saveptr);
  for (; sp && sp->next; sp = sp->next);
  s_triangle *new = malloc(sizeof (s_triangle));
  new->a.x = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->a.y =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->a.z =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->b.x =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->b.y =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->b.z =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->c.x =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->c.y =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->c.z =  strtof(bf, NULL);
  new->spe = fill_spe(saveptr);
  new->next = NULL;
  sp != NULL ? (sp->next = new) : (sc->triangle = new);
  return sc->triangle;
}

/**
** Fill a new struct plane which will be at the last position in the list
** of planes.
*/
s_plane *fill_plane(s_scene *sc, char **saveptr)
{
  s_plane *sp = sc->plane;
  char *bf = strtok_r(NULL, " ", saveptr);
  if (sc->plane)
  {
    while (sp->next)
      sp = sp->next;
  }
  s_plane *new = malloc(sizeof (s_plane));
  new->a = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->b =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->c =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->d =  strtof(bf, NULL);
  new->spe = fill_spe(saveptr);
  new->next = NULL;
  if (sp)
    sp->next = new;
  else
    return new;
  return sc->plane;
}

/**
** Fill a new struct plights which will be at the last position in the list
** of plights.
*/
s_plight *fill_plight(s_scene *sc, char **saveptr)
{
  s_plight *sp = sc->plight;
  char *bf = strtok_r(NULL, " ", saveptr);
  if (sc->plight)
  {
    while (sp->next)
      sp = sp->next;
  }
  s_plight *new = malloc(sizeof (s_plight));
  new->pos.x =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->pos.y =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->pos.z =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  s_color col;
  col.r =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.g =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.b =  strtof(bf, NULL);
  new->color = col;
  new->next = NULL;
  if (sp)
    sp->next = new;
  else
    return new;
  return sc->plight;
}

/**
** Fill a new struct dlight which will be at the last position in the list
** of dlight.
*/
s_dlight *fill_dlight(s_scene *sc, char **saveptr)
{
  s_dlight *sp = sc->dlight;
  char *bf = strtok_r(NULL, " ", saveptr);
  if (sc->dlight)
  {
    while (sp->next)
      sp = sp->next;
  }
  s_dlight *new = malloc(sizeof (s_plight));
  new->dir.x =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->dir.y =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  new->dir.z =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  s_color col;
  col.r =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.g =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.b =  strtof(bf, NULL);
  new->color = col;
  new->next = NULL;
  if (sp)
    sp->next = new;
  else
    return new;
  return sc->dlight;
}

/**
** Fill a new struct alight which will be at the last position in the list
** of alight.
*/
s_alight *fill_alight(s_scene *sc, char **saveptr)
{
  s_alight *sp = sc->alight;
  char *bf = strtok_r(NULL, " ", saveptr);
  if (sc->alight)
  {
    while (sp->next)
      sp = sp->next;
  }
  s_alight *new = malloc(sizeof (s_alight));
  s_color col;
  col.r =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.g =  strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  col.b =  strtof(bf, NULL);
  new->color = col;
  new->next = NULL;
  if (sp)
    sp->next = new;
  else
    return new;
  return sc->alight;
}

/**
** Initialize a new scene with all his value declared to NULL.
*/
static s_scene *init_scene(void)
{
  s_scene *scene = malloc(sizeof (s_scene));
  scene->sphere = NULL;
  scene->plane = NULL;
  scene->triangle = NULL;
  scene->plight = NULL;
  scene->dlight = NULL;
  scene->alight = NULL;
  return scene;
}

/**
** Main function of parsing the file name "pathname".
** This file must be correct.
** It will fill all the struct that the input wants.
*/
s_scene *parse(char *filename)
{
  FILE *f = fopen(filename, "r");
  s_scene *scene = init_scene();
  char *buf = malloc(256);
  char *aux = buf;

  for (buf = fgets(buf, 256, f); buf; buf = fgets(buf, 256, f))
  {
    char **saveptr = malloc(256);
    char *type = strtok_r(buf, " ", saveptr);
    if (!strcmp(type, "screen"))
      scene->screen = fill_screen(saveptr);
    else if (!strcmp(type, "camera"))
      scene->camera = fill_camera(saveptr);
    else if (!strcmp(type, "sphere"))
      scene->sphere = fill_sphere(scene, saveptr);
    else if (!strcmp(type, "plane"))
      scene->plane = fill_plane(scene, saveptr);
    else if (!strcmp(type, "triangle"))
      scene->triangle = fill_triangle(scene, saveptr);
    else if (!strcmp(type, "plight"))
      scene->plight = fill_plight(scene, saveptr);
    else if (!strcmp(type, "dlight"))
      scene->dlight = fill_dlight(scene, saveptr);
    else if (!strcmp(type, "alight"))
      scene->alight = fill_alight(scene, saveptr);
    free(saveptr);
  }
  free(aux);
  fclose(f);
  return scene;
}
