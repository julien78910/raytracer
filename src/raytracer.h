#ifndef RAY_TRACER_H
# define RAY_TRACER_H

static s_color **ray_tracer(s_scene *scene);
static s_color pixel_color(s_scene *scene, s_vec3 pixel);
/*static s_sphere *closest_sphere(s_scene *scene, s_vec3 pixel,
    s_vec3 dir, s_vec3 **clos_sphere);
static s_plane *closest_plane(s_scene *scene, s_vec3 pixel,
    s_vec3 dir, s_vec3 **clos_plane);
static s_triangle *closest_triangle(s_scene *scene, s_vec3 pixel,
    s_vec3 dir, s_vec3 **clos_triangle);
//static s_vec3 *closest_obj(s_sphere *sphere, s_plane *plane,
  //  s_triangle *triangle, s_spe *spe);
//static s_vec3 find_normal(s_sphere *sphere, s_plane *plane,
  //  s_triangle *triangle, s_vec3 closest_point);
//static s_color set_color(s_scene *sc, s_spe spe, s_vec3 nor, s_vec3 clos_inter);*/

#endif /* !RAY_TRACER_H */
