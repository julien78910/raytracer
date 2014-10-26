#ifndef LIGHTING_H
# define LIGHTING_H

s_color ambient_light(s_alight *alight, s_spe spe);
s_color point_light(s_plight *plight, s_spe spe, s_vec3 point, s_vec3 normal);
s_color dir_light(s_dlight *dlight, s_spe spe, s_vec3 normal);
s_color specular_lighting(s_vec3 norm, s_scene *sc, s_vec3 pixel, s_spe spe);

#endif /* !LIGHTING_H */
