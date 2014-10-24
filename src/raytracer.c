#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "data.h"
#include "vector3.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;

  s_scene *scene = parse(argv[1]);

  ray_tracer(scene);

}

void ray_tracer(s_scene *scene)
{
  s_camera cam = scene->camera;
  s_screen screen = scene->screen;

  s_vec3 vec_u = normalize(cam.u);
  s_vec3 vec_v = normalize(cam.v);
  s_vec3 vec_w = cross_prod(vec_u, vec_v);

  float dist_l = (screen.width / 2) / tan(M_PI / 8);
  s_vec3 center = add(cam.pos, scale(dist_l, vec_w));

  for (int w = -screen.width / 2; w < screen.width / 2; w++)
  {
    for (int h = -screen.height / 2; h < screen.height / 2; h++)
    {
      s_vec3 point = add(vec_u, vec_v);
      point.x *= w;
      point.y *= h;

      s_vec3 out = compute(cam.pos, point);
    }
  }
}
