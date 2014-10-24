#include "parser.h"
#include "data.h"
#include "vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static s_screen fill_screen(char **saveptr)
{
  char *bf = strtok_r(NULL, " ", saveptr);
  s_screen scr;
  scr.width = strtof(bf, NULL);
  bf = strtok_r(NULL, " ", saveptr);
  scr.height = strtof(bf, NULL);
  return scr;
}

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

s_sphere fill_sphere(s_scene sc, char **saveptr)
{
  s_phere *sp = sc->sphere;
  char *bf = strtok_r(NULL, " ", saveptr);
  if (sp)
  {
    while (sp->next)
      sp = sp->next;
    s_sphere *new = malloc(sizeof (s_sphere));
    new->radius = strtof(bf, NULL);
    bf = strtok_r(NULL, " ", saveptr);
    new->pos.x =  strtof(bf, NULL);
    bf = strtok_r(NULL, " ", saveptr);
    new->pos.y =  strtof(bf, NULL);
    bf = strtok_r(NULL, " ", saveptr);
    new->pos.z =  strtof(bf, NULL);
    bf = strtok_r(NULL, " ", saveptr);
    new->diff =  strtof(bf, NULL);
 bf = strtok_r(NULL, " ", saveptr);
    new->refl =  strtof(bf, NULL);
 bf = strtok_r(NULL, " ", saveptr);
    new->spec =  strtof(bf, NULL);
 bf = strtok_r(NULL, " ", saveptr);
    new->shin =  strtof(bf, NULL);
 bf = strtok_r(NULL, " ", saveptr);
    new->refr =  strtof(bf, NULL);
 bf = strtok_r(NULL, " ", saveptr);
    new->opac =  strtof(bf, NULL);


  }
  return sc->sphere;
}

s_scene *parse(char *filename)
{
  FILE *f = fopen(filename, r);
  s_scene *scene = malloc(sizeof (s_scene));
  int size = 500;
  char *buf = malloc(size * sizeof (char));
  buf = fgets(buf, size, f);
  while (buf)
  {
    char **saveptr = NULL;
    char *type = strtok_r(buf, " ", saveptr);
    if (!strcmp(type, "screen"))
    {
      scene->screen = fill_screen(saveptr);
    }
    else if (!strcmp(type, "camera"))
    {
      scene->camera = fill_camera(saveptr);
    }
    else if (!strcmp(type, "sphere"))
    {
      scene->sphere = fill_sphere(scene, saveptr);
    }
    else if (!strcmp(type, "plane"))
    {

    }
    else if (!strcmp(type, "triangle"))
    {

    }
    else if (!strcmp(type, "plight"))
    {

    }
    else if (!strcmp(type, "dlight"))
    {

    }
    else if (!strcmp(type, "alight"))
    {

    }
    buf = fgets(buf, size, f);
  }

  fclose(f);
  return scene;
}
