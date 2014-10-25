#ifndef RAY_TRACER_H
# define RAY_TRACER_H

s_color **ray_tracer(s_scene *scene);
s_color dir_light(s_dlight *dlight, s_spe spe, s_vec3 normal);
s_color point_light(s_plight *plight, s_spe spe, s_vec3 point, s_vec3 normal);
s_color ambient_light(s_alight *alight, s_spe spe);
s_vec3 *plane_intersec(s_vec3 dir, s_vec3 ray_pos, s_plane *plane);
s_vec3 *sphere_intersec(s_vec3 dir, s_vec3 ray_pos,
    s_vec3 intersec, s_sphere *sphere);
s_vec3 *triangle_intersec(s_vec3 dir, s_vec3 ray_pos, s_triangle *triangle);
s_color pixel_color(s_scene *scene, s_vec3 pixel);

#endif /* !RAY_TRACER_H */
