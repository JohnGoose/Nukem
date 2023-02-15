/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/05 21:52:54 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	normal_map(float u, float v, t_tri t, t_cast_result *res)
{
	int		x;
	int		y;
	float	w;

	w = 1 - u - v;
	x = t.verts[0].txtr.x * res->normal_map->width * w
		+ t.verts[1].txtr.x * res->normal_map->width * v
		+ t.verts[2].txtr.x * res->normal_map->width * u;
	y = t.verts[0].txtr.y * res->normal_map->height * w
		+ t.verts[1].txtr.y * res->normal_map->height * v
		+ t.verts[2].txtr.y * res->normal_map->height * u;
	wrap_coords_inverted(&x, &y, res->normal_map->width,
		res->normal_map->height);
	res->normal
		= get_normal(res->normal_map->image[x + (y * res->normal_map->width)]);
}

void	baked_color(t_cast_result *res, int x, int y)
{
	res->color = brightness(
			res->color >> 8, res->baked[x + y * res->texture->width])
		+ (res->color << 24 >> 24);
	res->light = res->baked[x + y * res->texture->width];
}

void	face_color(float u, float v, t_tri t, t_cast_result *res)
{
	int		x;
	int		y;
	float	w;

	w = 1 - u - v;
	x = t.verts[0].txtr.x * res->texture->width * w
		+ t.verts[1].txtr.x * res->texture->width * v
		+ t.verts[2].txtr.x * res->texture->width * u;
	y = t.verts[0].txtr.y * res->texture->height * w
		+ t.verts[1].txtr.y * res->texture->height * v
		+ t.verts[2].txtr.y * res->texture->height * u;
	wrap_coords_inverted(&x, &y, res->texture->width, res->texture->height);
	res->color = res->texture->image[x + (y * res->texture->width)];
	if (!t.dynamic && res->spray_overlay
		&& res->spray_overlay[x + y * res->texture->width])
		res->color = res->spray_overlay[x + y * res->texture->width];
	if (!t.dynamic && res->baked && !res->raytracing
		&& !t.isgrid && t.shader == SHADER_NONE)
		baked_color(res, x, y);
	if (!t.dynamic && res->normal_map)
		normal_map(u, v, t, res);
	else
		res->normal = t.normal;
}
