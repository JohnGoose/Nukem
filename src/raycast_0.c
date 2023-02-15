/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/05 21:39:40 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	rot_cam(t_vec3 *cam, const float lon, const float lat)
{
	float		phi;
	float		radius;

	phi = (M_PI / 2 - lat);
	radius = 1;
	cam->x = radius * sin(phi) * cos(lon);
	cam->y = radius * cos(phi);
	cam->z = radius * sin(phi) * sin(lon);
}

void	trace_bounce(t_cast_result *res, t_obj *obj, t_level *l)
{
	float	opacity_value;

	if (l->all.tris[res->face_index].reflectivity
		&& res->reflection_depth < REFLECTION_DEPTH)
	{
		res->reflection_depth++;
		if (res->reflection_depth == 1)
			reflection(res, l,
				&l->all.tris[res->face_index].reflection_obj_first_bounce);
		else
			reflection(res, l,
				&l->all.tris[res->face_index].reflection_obj_all);
	}
	opacity_value = 1.0 - (res->color << 24 >> 24) / (float)0xff;
	if (!opacity_value)
		opacity_value = l->all.tris[res->face_index].opacity;
	if (opacity_value && res->reflection_depth < REFLECTION_DEPTH)
	{
		res->reflection_depth++;
		opacity(res, l, obj, opacity_value);
	}
}

void	add_lightness(t_level *l, t_color light, t_cast_result *res)
{
	if (light.r - l->world_brightness > l->ui.bloom_limit
		|| light.g - l->world_brightness > l->ui.bloom_limit
		|| light.b - l->world_brightness > l->ui.bloom_limit)
	{
		res->light.r += light.r - l->world_brightness;
		res->light.g += light.g - l->world_brightness;
		res->light.b += light.b - l->world_brightness;
	}
}

void	raytrace(t_cast_result *res, t_obj *obj, t_level *l)
{
	t_color	light;
	t_vec3	tmp;

	tmp = res->ray.pos;
	vec_normalize(&res->normal);
	if (l->all.tris[res->face_index].shader == SHADER_PERLIN)
		res->color = shader_perlin(tmp, l, res);
	if (l->all.tris[res->face_index].shader == SHADER_RULE_30)
		res->color = shader_rule30(tmp);
	else if (!res->baked || res->raytracing || res->dynamic
		|| l->all.tris[res->face_index].isgrid
		|| l->all.tris[res->face_index].shader != SHADER_NONE)
	{
		light = sunlight(l, res, lights(l, res));
		res->color
			= brightness(res->color >> 8, light) + (res->color << 24 >> 24);
		add_lightness(l, light, res);
	}
	trace_bounce(res, obj, l);
}

int	cast_loop(t_obj *obj, t_cast_result *res)
{
	float	tmp_dist;
	float	min_dist;
	int		new_hit;
	int		i;
	t_vec2	uv;

	i = 0;
	min_dist = FLT_MAX;
	new_hit = -1;
	while (i < obj->tri_amount)
	{
		tmp_dist = cast_face(obj->tris[i], res->ray, res);
		if (0 < tmp_dist && tmp_dist < min_dist
			&& obj->tris[i].index != res->face_index)
		{
			min_dist = tmp_dist;
			new_hit = i;
			uv = res->uv;
		}
		i++;
	}
	res->uv = uv;
	res->dist = min_dist;
	return (new_hit);
}
