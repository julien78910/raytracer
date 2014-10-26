#ifndef INTERSEC_H
# define INTERSEC_H

s_vec3 *sphere_intersec(s_vec3 dir, s_vec3 ray_pos,
    s_vec3 intersec, s_sphere *sphere);
s_vec3 *plane_intersec(s_vec3 dir, s_vec3 ray_pos, s_plane *plane);
s_vec3 *triangle_intersec(s_vec3 dir, s_vec3 ray_pos, s_triangle *triangle);

#endif /* INTERSEC_H */
