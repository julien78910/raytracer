#ifndef RAY_TRACER_H
# define RAY_TRACER_H

s_color **ray_tracer(s_scene *scene);
s_vec3 sphere_intersec(s_vec3 dir, s_scene *scene, s_vec3 point, s_sphere *obj);
s_color dir_light(s_scene *scene, s_sphere *obj, s_vec3 point);
s_color point_light(s_scene *scene, s_sphere *obj, s_vec3 point);
s_color ambient_light(s_scene *scene, s_sphere *closest);
s_vec3 plane_intersec(s_vec3 dir, s_vec3 ray_pos, s_plane *plane);

#endif /* !RAY_TRACER_H */
