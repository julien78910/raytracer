#include "destroyer.h"
#include "data.h"
#include "vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
** Destroy the whole list of spheres.
*/
static void destroy_sphere(s_sphere *sphere)
{
  if (sphere)
  {
    destroy_sphere(sphere->next);
    free(sphere);
  }
}

/**
** Destroy the whole list of planes.
*/
static void destroy_plane(s_plane *plane)
{
  if (plane)
  {
    destroy_plane(plane->next);
    free(plane);
  }
}

/**
** Destroy the whole list of triangles.
*/
static void destroy_triangle(s_triangle *triangle)
{
  if (triangle)
  {
    destroy_triangle(triangle->next);
    free(triangle);
  }
}

/**
** Destroy the whole list of plights.
*/
static void destroy_plight(s_plight *plight)
{
  if (plight)
  {
    destroy_plight(plight->next);
    free(plight);
  }
}

/**
** Destroy the whole list of dlights.
*/
static void destroy_dlight(s_dlight *dlight)
{
  if (dlight)
  {
    destroy_dlight(dlight->next);
    free(dlight);
  }
}

/**
** Destroy the whole list of alights.
*/
static void destroy_alight(s_alight *alight)
{
  if (alight)
  {
    destroy_alight(alight->next);
    free(alight);
  }
}

static void destroy_output(s_screen screen, s_color **output)
{
   for (int i = 0; i < screen.width; i++)
    free(output[i]);

   free(output);
}
/**
** Destroy the whole scene.
*/
void destroy_all(s_scene *scene, s_color **output)
{
  destroy_sphere(scene->sphere);
  destroy_plane(scene->plane);
  destroy_triangle(scene->triangle);
  destroy_plight(scene->plight);
  destroy_dlight(scene->dlight);
  destroy_alight(scene->alight);
  destroy_output(scene->screen, output);
  free(scene);
}

