/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bake_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	bake_pixel(t_level *l, t_ivec2 wrap, t_ivec2 i, t_cast_result *res)
{
	res->ray.pos = uv_to_3d(l->all.tris[res->face_index], &l->texture, i);
	if (res->normal_map)
	{
		res->normal = get_normal(res->normal_map->image[wrap.x
				+ (wrap.y * res->normal_map->width)]);
		vec_normalize(&res->normal);
	}
	else
		res->normal = l->all.tris[res->face_index].normal;
	l->baked[wrap.x + wrap.y * l->texture.width] = sunlight(l, res,
			lights(l, res));
}

int	point_in_face(t_tri tri, t_vec2 point)
{
	return ((tri.isquad && point_in_tri(point, tri.verts[3].txtr,
				tri.verts[1].txtr, tri.verts[2].txtr))
		|| point_in_tri(point, tri.verts[0].txtr,
			tri.verts[1].txtr, tri.verts[2].txtr));
}

void	fill_area(t_level *l, t_ivec2 start, t_color c, int res)
{
	t_ivec2	i;
	int		x;
	int		y;

	i.x = -1;
	while (++i.x < res)
	{
		i.y = -1;
		while (++i.y < res)
		{
			x = start.x + i.x;
			y = start.y + i.y;
			if (x < l->texture.width && y < l->texture.height && x > 0 && y > 0
				&& !l->baked[x + y * l->texture.width].r
				&& !l->baked[x + y * l->texture.width].g
				&& !l->baked[x + y * l->texture.width].b)
				l->baked[x + y * l->texture.width] = c;
		}
	}
}

void	bake_area(t_level *l, t_cast_result *res, t_ivec2 i)
{
	t_vec2	tmp;
	t_ivec2	wrap;
	t_tri	tri;

	tmp.x = (float)i.x / l->texture.width;
	tmp.y = 1 - (float)i.y / l->texture.height;
	tri = l->all.tris[res->face_index];
	if (point_in_face(tri, tmp))
	{
		wrap = i;
		wrap_coords(&wrap.x, &wrap.y, l->texture.width, l->texture.height);
		bake_pixel(l, wrap, i, res);
		i.x -= l->ui.bake_quality;
		i.y -= l->ui.bake_quality;
		fill_area(l, i, l->baked[wrap.x + wrap.y * l->texture.width],
			l->ui.bake_quality * 2);
	}
}

void	bake_face(t_cast_result *res, t_level *l)
{
	t_ivec2			i;
	t_vec2			min;
	t_vec2			max;

	texture_minmax(&min, &max, l->all.tris[res->face_index]);
	i.x = min.x * l->texture.width;
	while (i.x < max.x * l->texture.width)
	{
		i.y = min.y * l->texture.height;
		while (i.y < max.y * l->texture.height)
		{
			if (l->bake_status != BAKE_BAKING)
				return ;
			bake_area(l, res, i);
			i.y += l->ui.bake_quality;
		}
		i.x += l->ui.bake_quality;
	}
}
