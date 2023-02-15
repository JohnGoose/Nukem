/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	opacity_full_check(t_cast_result *res, t_level *l, t_obj *obj,
										float opacity)
{
	if (opacity == 1.0)
	{
		cast_all_color(l, obj, res, FALSE);
		return (res->dist);
	}
	else
		cast_all_color(l, obj, res, TRUE);
	return (0);
}

void	opacity(t_cast_result *res, t_level *l, t_obj *obj, float opacity)
{
	t_cast_result	transparent;
	t_ray			normal;

	transparent = *res;
	transparent.dist = 0;
	vec_normalize(&transparent.ray.dir);
	if (l->all.tris[res->face_index].refractivity == 0)
		res->dist += opacity_full_check(&transparent, l, obj, opacity);
	else
	{
		normal.pos = res->ray.pos;
		normal.dir = res->normal;
		vec_mult(&normal.dir, vec_dot(transparent.ray.dir, normal.dir)
			* l->all.tris[res->face_index].refractivity);
		vec_add(&transparent.ray.dir, transparent.ray.dir, normal.dir);
		vec_normalize(&transparent.ray.dir);
		if (l->all.tris[res->face_index].opacity_precise)
			cast_all_color(l, &l->all.tris[res->face_index].opacity_obj_all,
				&transparent, TRUE);
		else
			cast_all_color(l, obj, &transparent, TRUE);
	}
	res->color = crossfade((unsigned int)res->color >> 8,
			(unsigned int)transparent.color >> 8,
			opacity * 0xff, opacity * 0xff);
}

t_color	sunlight(t_level *l, t_cast_result *res, t_color light)
{
	float			res_brightness;
	int				i;
	t_ray			r;

	if (vec_dot(res->normal, l->ui.sun_dir) < 0
		|| (l->ui.sun_color.r == 0
			&& l->ui.sun_color.g == 0
			&& l->ui.sun_color.b == 0))
		return (light);
	r.dir = l->ui.sun_dir;
	r.pos = res->ray.pos;
	i = -1;
	if (res->raytracing)
		while (++i < l->all.tris[res->face_index].shadow_faces.tri_amount)
			if (0 < cast_face(l->all.tris[res->face_index].shadow_faces.tris[i],
					r, NULL))
				return (light);
	res_brightness = vec_dot(res->normal, l->ui.sun_dir);
	light.r += res_brightness * l->ui.sun_color.r;
	light.g += res_brightness * l->ui.sun_color.g;
	light.b += res_brightness * l->ui.sun_color.b;
	return (light);
}

void	color_set(t_color *col, float value)
{
	col->r = value;
	col->g = value;
	col->b = value;
}

t_color	lights(t_level *l, t_cast_result *res)
{
	t_ray	ray;
	float	dist;
	t_color	col;
	int		i;

	i = -1;
	color_set(&col, l->world_brightness);
	while (++i < l->light_amount)
	{
		vec_sub(&ray.dir, res->ray.pos, l->lights[i].pos);
		dist = 1.0 - vec_length(ray.dir) / l->lights[i].radius;
		ray.pos = l->lights[i].pos;
		if (dist > 0 && vec_dot(ray.dir, res->normal) < 0 && (!res->raytracing
				|| cast_all(ray, l, NULL) >= vec_length(ray.dir) - 0.1))
		{
			vec_normalize(&ray.dir);
			dist = dist * l->lights[i].power * -vec_dot(ray.dir, res->normal);
			col.r += dist * l->lights[i].color.r;
			col.g += dist * l->lights[i].color.g;
			col.b += dist * l->lights[i].color.b;
		}
	}
	return (col);
}
