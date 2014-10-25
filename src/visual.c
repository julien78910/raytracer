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
      int r = output[j][i].r;
      int g = output[j][i].g;
      int b = output[j][i].b;
      fprintf(f, "%d %d %d  ", r, g, b);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}
