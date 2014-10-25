#include "visual.h"
#include "parser.h"
#include "data.h"
#include "vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
 *  Create an output file with the format .PPM, in order to visualize our
 *  Raytracer.
 *  ****/
void display(char *filename, s_color **color, s_screen screen)
{
  FILE *f = fopen(filename, "w");
  fprintf(f, "P3\n");
  fprintf(f, "%d %d\n255\n", screen.width, screen.height);
  for (int i = 0; i < screen.height; i++)
  {
    for (int j = 0; j < screen.width; j++)
    {
      int r = color[j][i].r;
      int g = color[j][i].g;
      int b = color[j][i].b;
      if (j < screen.width - 1)
        fprintf(f, "%d %d %d  ", r, g, b);
      else
        fprintf(f, "%d %d %d", r, g, b);
    }
    if (i < screen.height - 1)
      fprintf(f, "\n");
  }
  fclose(f);
}
