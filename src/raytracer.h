#ifndef RAY_TRACER_H
# define RAY_TRACER_H

s_color *ray_tracer(s_scene *scene);
s_vec3 find_intersec(s_vec3 dir, s_scene *scene, s_vec3 point);

#endif /* !RAY_TRACER_H */
